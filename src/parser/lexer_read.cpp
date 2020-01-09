/**
 * \file parser/lexer_read.cpp
 *
 * \brief Read a token.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <istream>

using namespace homesim;
using namespace std;

/**
 * \brief Read a token from the stream.
 *
 * \returns the token read.
 *      - HOMESIM_TOKEN_INVALID if invalid data was read from the stream.
 *      - HOMESIM_TOKEN_EOF if at the end of the stream.
 */
token homesim::lexer::read()
{
    int ch;

    /* skip whitespace. */
    do { ch = in.get(); } while (isspace(ch));

    switch (ch)
    {
        case EOF:
            tokenbuf.clear();
            return HOMESIM_TOKEN_EOF;

        case ':':
            start(ch);
            return maybeReadAssign();

        case '*':
            start(ch);
            return maybeReadAsterisk();

        case '{':
            start(ch);
            return HOMESIM_TOKEN_BRACE_LEFT;

        case '}':
            start(ch);
            return HOMESIM_TOKEN_BRACE_RIGHT;

        case '[':
            start(ch);
            return HOMESIM_TOKEN_BRACKET_LEFT;

        case ']':
            start(ch);
            return HOMESIM_TOKEN_BRACKET_RIGHT;

        case '(':
            start(ch);
            return HOMESIM_TOKEN_PAREN_LEFT;

        case ')':
            start(ch);
            return HOMESIM_TOKEN_PAREN_RIGHT;

        case '.':
            start(ch);
            return maybeReadDotOrNumber();

        case '-':
            start(ch);
            return maybeReadNumberWithAtLeastOneDigit();

        case '=':
            start(ch);
            return HOMESIM_TOKEN_EQUALS;

        case '"':
            start(ch);
            return maybeReadString();

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            start(ch);
            return maybeReadNumber();

        case 'a':
            start(ch);
            return maybeReadKeywordAfterAssertAtOrIdentifier();

        case 'c':
            start(ch);
            return
                matchKeywordOrIdentifier(
                    "omponent", HOMESIM_TOKEN_KEYWORD_COMPONENT);

        case 'e':
            start(ch);
            return
                maybeReadKeywordExamineExecutionExpectExportExternalOrIdentifier();

        case 'f':
            start(ch);
            return
                matchKeywordOrIdentifier("alse", HOMESIM_TOKEN_KEYWORD_FALSE);

        case 'm':
            start(ch);
            return
                matchKeywordOrIdentifier("odule", HOMESIM_TOKEN_KEYWORD_MODULE);

        case 'p':
            start(ch);
            return maybeReadKeywordPinProbePulldownPullupOrIdentifier();

        case 's':
            start(ch);
            return maybeReadKeywordScenarioSignalStartStateOrIdentifier();

        case 't':
            start(ch);
            return maybeReadKeywordTrueTypeOrIdentifier();

        case 'w':
            start(ch);
            return matchKeywordOrIdentifier("ire", HOMESIM_TOKEN_KEYWORD_WIRE);

        default:
            in.putback(ch);
            return maybeReadStartIdentifier();
    }
}

token homesim::lexer::maybeReadAssign()
{
    int ch = in.get();

    accept(ch);

    /* after a colon, only an equal is valid. */
    if (ch != '=')
        return HOMESIM_TOKEN_INVALID;

    return HOMESIM_TOKEN_ASSIGN;
}

token homesim::lexer::maybeReadAsterisk()
{
    int ch = in.get();

    /* after an asterisk, a whitespace character terminates the read. */
    if (isspace(ch))
        return HOMESIM_TOKEN_ASTERISK;

    /* EOF is also valid. */
    if (EOF == ch)
        return HOMESIM_TOKEN_ASTERISK;

    /* we can't consume any other character, as they might be part of another
     * token. */
    in.putback(ch);

    /* determine whether this represents a compatible following token. */
    switch (ch)
    {
        /* structural tokens are fine. */
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            return HOMESIM_TOKEN_ASTERISK;

        /* anything else is not. */
        default:
            accept(ch);
            return HOMESIM_TOKEN_INVALID;
    }
}

token homesim::lexer::maybeReadDotOrNumber()
{
    int ch = in.get();

    /* after a dot, a whitespace character terminates the read. */
    if (isspace(ch))
        return HOMESIM_TOKEN_DOT;

    /* EOF is also valid. */
    if (EOF == ch)
        return HOMESIM_TOKEN_DOT;

    /* if the character is a digit, then this starts a number read. */
    if (isdigit(ch))
    {
        accept(ch);
        return maybeReadDecimalNumber();
    }

    /* for any other character, this is a dot. */
    in.putback(ch);

    return HOMESIM_TOKEN_DOT;
}

token homesim::lexer::maybeReadNumberWithAtLeastOneDigit()
{
    int ch = in.get();

    if (EOF == ch)
        return HOMESIM_TOKEN_INVALID;
    
    accept(ch);

    if (isdigit(ch))
    {
        return maybeReadNumber();
    }

    /* any other character is invalid. */
    in.putback(ch);

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadNumber()
{
    int ch = in.get();

    while (isdigit(ch))
    {
        accept(ch);
        ch = in.get();
    }

    /* a space terminates the number. */
    if (isspace(ch))
        return HOMESIM_TOKEN_NUMBER;

    switch (ch)
    {
        /* e or E start an exponent. */
        case 'E':
        case 'e':
            accept(ch);
            return maybeReadExponentNumberWithSign();

        /* structural tokens or EOF are fine. */
        case EOF:
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            in.putback(ch);
            return HOMESIM_TOKEN_NUMBER;

        /* equals is okay. */
        case '=':
            in.putback(ch);
            return HOMESIM_TOKEN_NUMBER;

        /* a dot continues the number in decimal form. */
        case '.':
            accept(ch);
            return maybeReadDecimalNumber();

        /* anything else is not. */
        default:
            accept(ch);
            in.putback(ch);
            return HOMESIM_TOKEN_INVALID;
    }
}

token homesim::lexer::maybeReadDecimalNumber()
{
    int ch = in.get();

    while (isdigit(ch))
    {
        accept(ch);
        ch = in.get();
    }

    /* a space terminates the number. */
    if (isspace(ch))
        return HOMESIM_TOKEN_NUMBER;

    switch (ch)
    {
        /* e or E start an exponent. */
        case 'E':
        case 'e':
            accept(ch);
            return maybeReadExponentNumberWithSign();

        /* structural tokens or EOF are fine. */
        case EOF:
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            in.putback(ch);
            return HOMESIM_TOKEN_NUMBER;

        /* equals is okay. */
        case '=':
            in.putback(ch);
            return HOMESIM_TOKEN_NUMBER;

        /* anything else is not. */
        default:
            accept(ch);
            in.putback(ch);
            return HOMESIM_TOKEN_INVALID;
    }

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadExponentNumberWithSign()
{
    int ch = in.get();

    /* a negative number can start an exponent. */
    if ('-' == ch)
    {
        accept(ch);
        return maybeReadExponentNumber();
    }

    /* if a digit is discovered, we can terminate with the exponent number. */
    if (isdigit(ch))
    {
        in.putback(ch);
        return maybeReadExponentNumber();
    }

    /* if eof occurs, this is an invalid token. */
    if (EOF == ch)
        return HOMESIM_TOKEN_INVALID;

    /* any other value is invalid. */
    accept(ch);
    in.putback(ch);

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadExponentNumber()
{
    int ch = in.get();

    /* read at least one digit. */
    if (!isdigit(ch))
    {
        if (EOF != ch)
        {
            accept(ch);
            in.putback(ch);
        }

        return HOMESIM_TOKEN_INVALID;
    }

    /* consume remaining digits. */
    while (isdigit(ch))
    {
        accept(ch);
        ch = in.get();
    }

    /* a space can successfully terminate the read. */
    if (isspace(ch))
        return HOMESIM_TOKEN_NUMBER;

    /* any other character is the start of something else. */
    in.putback(ch);

    switch (ch)
    {
        /* structural tokens or EOF are fine. */
        case EOF:
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            return HOMESIM_TOKEN_NUMBER;

        /* equals is okay. */
        case '=':
            return HOMESIM_TOKEN_NUMBER;

        /* anything else is not. */
        default:
            return HOMESIM_TOKEN_INVALID;
    }
}

token homesim::lexer::maybeReadStartIdentifier()
{
    int ch = in.get();

    start(ch);

    if (isalpha(ch) || '_' == ch)
        return maybeReadIdentifier();

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadIdentifier()
{
    int ch = in.get();

    while (isalnum(ch) || '_' == ch)
    {
        accept(ch);
        ch = in.get();
    }

    /* a space successfully terminates an identifier. */
    if (isspace(ch))
        return HOMESIM_TOKEN_IDENTIFIER;

    /* any other value is something else. */
    in.putback(ch);

    switch (ch)
    {
        /* structural tokens or EOF are fine. */
        case EOF:
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            return HOMESIM_TOKEN_IDENTIFIER;

        /* equals is okay. */
        case '=':
            return HOMESIM_TOKEN_IDENTIFIER;

        /* dot is okay. */
        case '.':
            return HOMESIM_TOKEN_IDENTIFIER;

        /* something fishy. */
        default:
            accept(ch);
            return HOMESIM_TOKEN_INVALID;
    }

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadKeywordAfterAssertAtOrIdentifier()
{
    int ch = in.get();

    if (isspace(ch))
        return HOMESIM_TOKEN_IDENTIFIER;

    switch (ch)
    {
        case 'f':
            accept(ch);
            return
                matchKeywordOrIdentifier("ter", HOMESIM_TOKEN_KEYWORD_AFTER);

        case 's':
            accept(ch);
            return
                matchKeywordOrIdentifier("sert", HOMESIM_TOKEN_KEYWORD_ASSERT);

        case 't':
            accept(ch);
            return
                matchKeywordOrIdentifier("", HOMESIM_TOKEN_KEYWORD_AT);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }

    return HOMESIM_TOKEN_INVALID;
}

token
homesim::lexer::maybeReadKeywordExamineExecutionExpectExportExternalOrIdentifier()
{
    int ch = in.get();

    if ('x' != ch)
    {
        in.putback(ch);
        return maybeReadIdentifier();
    }

    accept(ch);
    ch = in.get();

    switch (ch)
    {
        case 'a':
            accept(ch);
            return
                matchKeywordOrIdentifier("mine", HOMESIM_TOKEN_KEYWORD_EXAMINE);

        case 'e':
            accept(ch);
            return
                matchKeywordOrIdentifier(
                    "cution", HOMESIM_TOKEN_KEYWORD_EXECUTION);

        case 'p':
            accept(ch);
            return maybeReadKeywordExpectExportOrIdentifier();

        case 't':
            accept(ch);
            return
                matchKeywordOrIdentifier(
                    "ernal", HOMESIM_TOKEN_KEYWORD_EXTERNAL);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadKeywordExpectExportOrIdentifier()
{
    int ch = in.get();

    switch (ch)
    {
        case 'e':
            accept(ch);
            return
                matchKeywordOrIdentifier("ct", HOMESIM_TOKEN_KEYWORD_EXPECT);

        case 'o':
            accept(ch);
            return
                matchKeywordOrIdentifier(
                    "rt", HOMESIM_TOKEN_KEYWORD_EXPORT);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }
}

token homesim::lexer::maybeReadKeywordPinProbePulldownPullupOrIdentifier()
{
    int ch = in.get();

    switch (ch)
    {
        case 'i':
            accept(ch);
            return matchKeywordOrIdentifier("n", HOMESIM_TOKEN_KEYWORD_PIN);

        case 'r':
            accept(ch);
            return matchKeywordOrIdentifier("obe", HOMESIM_TOKEN_KEYWORD_PROBE);

        case 'u':
            accept(ch);
            return maybeReadKeywordPulldownPullupOrIdentifier();

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadKeywordPulldownPullupOrIdentifier()
{
    int ch = in.get();

    if ('l' != ch)
    {
        in.putback(ch);
        return maybeReadIdentifier();
    }

    accept(ch);
    ch = in.get();

    if ('l' != ch)
    {
        in.putback(ch);
        return maybeReadIdentifier();
    }

    accept(ch);
    ch = in.get();

    switch (ch)
    {
        case 'd':
            accept(ch);
            return
                matchKeywordOrIdentifier("own", HOMESIM_TOKEN_KEYWORD_PULLDOWN);

        case 'u':
            accept(ch);
            return matchKeywordOrIdentifier("p", HOMESIM_TOKEN_KEYWORD_PULLUP);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }

    return HOMESIM_TOKEN_INVALID;
}

token homesim::lexer::maybeReadKeywordScenarioSignalStartStateOrIdentifier()
{
    int ch = in.get();

    switch (ch)
    {
        case 'c':
            accept(ch);
            return
                matchKeywordOrIdentifier(
                    "enario", HOMESIM_TOKEN_KEYWORD_SCENARIO);

        case 'i':
            accept(ch);
            return
                matchKeywordOrIdentifier("gnal", HOMESIM_TOKEN_KEYWORD_SIGNAL);

        case 'o':
            accept(ch);
            return
                matchKeywordOrIdentifier("urce", HOMESIM_TOKEN_KEYWORD_SOURCE);

        case 't':
            accept(ch);
            return maybeReadKeywordStartStateOrIdentifier();

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }
}

token homesim::lexer::maybeReadKeywordStartStateOrIdentifier()
{
    int ch = in.get();

    if ('a' != ch)
    {
        in.putback(ch);
        return maybeReadIdentifier();
    }

    accept(ch);
    ch = in.get();

    switch (ch)
    {
        case 'r':
            accept(ch);
            return matchKeywordOrIdentifier("t", HOMESIM_TOKEN_KEYWORD_START);

        case 't':
            accept(ch);
            return matchKeywordOrIdentifier("e", HOMESIM_TOKEN_KEYWORD_STATE);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }
}

token homesim::lexer::maybeReadKeywordTrueTypeOrIdentifier()
{
    int ch = in.get();

    switch (ch)
    {
        case 'r':
            accept(ch);
            return matchKeywordOrIdentifier("ue", HOMESIM_TOKEN_KEYWORD_TRUE);

        case 'y':
            accept(ch);
            return matchKeywordOrIdentifier("pe", HOMESIM_TOKEN_KEYWORD_TYPE);

        case EOF:
            return HOMESIM_TOKEN_IDENTIFIER;

        default:
            in.putback(ch);
            return maybeReadIdentifier();
    }
}

token homesim::lexer::maybeReadString()
{
    int ch = in.get();

    while ('"' != ch && EOF != ch)
    {
        accept(ch);
        ch = in.get();
    }

    if (EOF == ch)
    {
        return HOMESIM_TOKEN_INVALID;
    }

    accept(ch);

    return HOMESIM_TOKEN_STRING;
}

token homesim::lexer::matchSequence(
    const string& seq, function<token ()> onAccept, function<token ()> onFail)
{
    for (auto i : seq)
    {
        int ch = in.get();

        if (ch != i)
        {
            in.putback(ch);
            return onFail();
        }

        accept(ch);
    }

    return onAccept();
}

token homesim::lexer::matchEndOfKeyword(
    function<token ()> onAccept,
    function<token ()> onFail)
{
    int ch = in.get();

    /* space ends the keyword. */
    if (isspace(ch))
        return onAccept();

    /* any other value turns this into an identifier. */
    if (isalnum(ch) || '_' == ch)
    {
        accept(ch);
        return onFail();
    }

    /* any other value is something else. */
    in.putback(ch);

    /* handle other possible ending characters. */
    switch (ch)
    {
        /* structural tokens or EOF are fine. */
        case EOF:
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            return onAccept();

        /* equals is okay. */
        case '=':
            return onAccept();

        /* dot is okay. */
        case '.':
            return onAccept();

        /* assignment is okay. */
        case ':':
            return onAccept();

        /* something fishy. */
        default:
            return onFail();
    }
}

token homesim::lexer::matchKeywordOrIdentifier(
    const std::string& seq, token keywordToken)
{
    return
        matchSequence(
            seq,
            [&]() -> token {
                return
                    matchEndOfKeyword(
                        [=]() -> token {
                            return keywordToken; },
                        [&]() -> token {
                            return maybeReadIdentifier(); });},
            [&]() -> token { return maybeReadIdentifier(); });
}
