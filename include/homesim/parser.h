/**
 * \file homesim/parser.h
 *
 * \brief Lexer and parser declarations for parsing homesim files or running the
 * homesim REPL.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_PARSER_HEADER_GUARD
# define HOMESIM_PARSER_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <iosfwd>
#include <list>
#include <string>

namespace homesim {

/**
 * \brief Tokens supported by the lexer.
 */
enum token
{
    /* assignment. */
    HOMESIM_TOKEN_ASSIGN,
    /* asterisk. */
    HOMESIM_TOKEN_ASTERISK,
    /* left brace. */
    HOMESIM_TOKEN_BRACE_LEFT,
    /* right brace. */
    HOMESIM_TOKEN_BRACE_RIGHT,
    /* left bracket. */
    HOMESIM_TOKEN_BRACKET_LEFT,
    /* right bracket. */
    HOMESIM_TOKEN_BRACKET_RIGHT,
    /* dot. */
    HOMESIM_TOKEN_DOT,
    /* equals. */
    HOMESIM_TOKEN_EQUALS,
    /* identifier. */
    HOMESIM_TOKEN_IDENTIFIER,
    /* after keyword. */
    HOMESIM_TOKEN_KEYWORD_AFTER,
    /* assert keyword. */
    HOMESIM_TOKEN_KEYWORD_ASSERT,
    /* at keyword. */
    HOMESIM_TOKEN_KEYWORD_AT,
    /* component keyword. */
    HOMESIM_TOKEN_KEYWORD_COMPONENT,
    /* examine keyword. */
    HOMESIM_TOKEN_KEYWORD_EXAMINE,
    /* execution keyword. */
    HOMESIM_TOKEN_KEYWORD_EXECUTION,
    /* expect keyword. */
    HOMESIM_TOKEN_KEYWORD_EXPECT,
    /* false keyword. */
    HOMESIM_TOKEN_KEYWORD_FALSE,
    /* module keyword. */
    HOMESIM_TOKEN_KEYWORD_MODULE,
    /* pin keyword. */
    HOMESIM_TOKEN_KEYWORD_PIN,
    /* probe keyword. */
    HOMESIM_TOKEN_KEYWORD_PROBE,
    /* pulldown keyword. */
    HOMESIM_TOKEN_KEYWORD_PULLDOWN,
    /* pullup keyword. */
    HOMESIM_TOKEN_KEYWORD_PULLUP,
    /* scenario keyword. */
    HOMESIM_TOKEN_KEYWORD_SCENARIO,
    /* signal keyword. */
    HOMESIM_TOKEN_KEYWORD_SIGNAL,
    /* start keyword. */
    HOMESIM_TOKEN_KEYWORD_START,
    /* state keyword. */
    HOMESIM_TOKEN_KEYWORD_STATE,
    /* true keyword. */
    HOMESIM_TOKEN_KEYWORD_TRUE,
    /* type keyword. */
    HOMESIM_TOKEN_KEYWORD_TYPE,
    /* wire keyword. */
    HOMESIM_TOKEN_KEYWORD_WIRE,
    /* decimal number (double). */
    HOMESIM_TOKEN_NUMBER,
    /* left paren. */
    HOMESIM_TOKEN_PAREN_LEFT,
    /* right paren. */
    HOMESIM_TOKEN_PAREN_RIGHT,
    /* quoted string. */
    HOMESIM_TOKEN_STRING,

    /* end of input. */
    HOMESIM_TOKEN_EOF,

    /* an invalid token. */
    HOMESIM_TOKEN_INVALID
};

/**
 * \brief The lexer tokenizes an input stream.  These tokens can then be passed
 * to a parser instance to parse a homesim stream.
 */
class lexer
{
public:

    /**
     * \brief Create a lexer instance backed by the given input stream.
     *
     * \param input         The input stream for this instance.
     */
    lexer(std::istream& input);

    /**
     * \brief Read a token from the stream.
     *
     * \returns the token read.
     *      - HOMESIM_TOKEN_INVALID if invalid data was read from the stream.
     *      - HOMESIM_TOKEN_EOF if at the end of the stream.
     */
    token read();

    /**
     * \brief Get the line and column information for the current token.
     *
     * \param start_line        This reference is set to the start line of the
     *                          token.
     * \param start_col         This reference is set to the start column of the
     *                          token.
     * \param end_line          This reference is set to the end line of the
     *                          token.
     * \param end_col           This reference is set to the end column of the
     *                          token.
     */
    void read_linecol(
        int& start_line, int& start_col, int& end_line, int& end_col) const;

    /**
     * \brief Get the string value of the current token.
     *
     * \returns string value of the token or an empty string if inappropriate.
     */
    std::string get_token_string();

private:
    std::istream& in;
    std::list<char> tokenbuf;
    int curline;
    int curcol;
    int start_line;
    int start_col;
    int end_line;
    int end_col;

    void start(int ch);
    void accept(int ch);

    token matchSequence(
        const std::string& seq, std::function<token ()> onAccept,
        std::function<token ()> onFail);

    token matchKeywordOrIdentifier(
        const std::string& seq, token keywordToken);

    token matchEndOfKeyword(
        std::function<token ()> onAccept,
        std::function<token ()> onFail);

    token maybeReadAssign();
    token maybeReadAsterisk();
    token maybeReadDotOrNumber();
    token maybeReadNumberWithAtLeastOneDigit();
    token maybeReadNumber();
    token maybeReadDecimalNumber();
    token maybeReadExponentNumberWithSign();
    token maybeReadExponentNumber();
    token maybeReadKeywordAfterAssertAtOrIdentifier();
    token maybeReadKeywordExamineExecutionExpectOrIdentifier();
    token maybeReadKeywordPinProbePulldownPullupOrIdentifier();
    token maybeReadKeywordPulldownPullupOrIdentifier();
    token maybeReadKeywordPullupOrIdentifier();
    token maybeReadKeywordScenarioSignalStartStateOrIdentifier();
    token maybeReadKeywordStartStateOrIdentifier();
    token maybeReadKeywordTrueTypeOrIdentifier();
    token maybeReadIdentifier();
    token maybeReadStartIdentifier();
    token maybeReadString();
};

} /* namespace homesim */

#endif /*HOMESIM_PARSER_HEADER_GUARD*/
