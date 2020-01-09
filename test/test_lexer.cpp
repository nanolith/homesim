/**
 * \file test/test_lexer.cpp
 *
 * \brief Unit tests for the lexer.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <memory>
#include <minunit/minunit.h>
#include <sstream>

using namespace homesim;
using namespace std;

TEST_SUITE(lexer);

/**
 * It's possible to scan EOF.
 */
TEST(eof_token)
{
    stringstream in("");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * It's possible to scan an assign token.
 */
TEST(assign_token)
{
    const string VALUE = ":=";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_ASSIGN == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * It's possible to scan an asterisk token.
 */
TEST(asterisk_token)
{
    const string VALUE = "*";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * An asterisk can't match if there isn't whitespace after it.
 */
TEST(asterisk_clutter)
{
    const string VALUE = "*f";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_INVALID == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
}

/**
 * Multiple asterisks can be read.
 */
TEST(asterisk_multiple)
{
    const string VALUE = "*";
    stringstream in("* *");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * An asterisk before a brace or bracket is valid.
 */
TEST(asterisk_brace_bracket_paren)
{
    auto in = make_shared<stringstream>("*{");
    auto scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACE_LEFT == scanner->read());
    TEST_EXPECT(string("{") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("*}");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACE_RIGHT == scanner->read());
    TEST_EXPECT(string("}") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("*[");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_LEFT == scanner->read());
    TEST_EXPECT(string("[") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("*]");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_RIGHT == scanner->read());
    TEST_EXPECT(string("]") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("*(");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_PAREN_LEFT == scanner->read());
    TEST_EXPECT(string("(") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("*)");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_ASTERISK == scanner->read());
    TEST_EXPECT(string("*") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_PAREN_RIGHT == scanner->read());
    TEST_EXPECT(string(")") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());
}

/**
 * A left brace can be read.
 */
TEST(left_brace)
{
    stringstream in("{");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_BRACE_LEFT == scanner.read());
    TEST_EXPECT(string("{") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A right brace can be read.
 */
TEST(right_brace)
{
    stringstream in("}");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_BRACE_RIGHT == scanner.read());
    TEST_EXPECT(string("}") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A left bracket can be read.
 */
TEST(left_bracket)
{
    stringstream in("[");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_LEFT == scanner.read());
    TEST_EXPECT(string("[") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A right bracket can be read.
 */
TEST(right_bracket)
{
    stringstream in("]");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_RIGHT == scanner.read());
    TEST_EXPECT(string("]") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A left paren can be read.
 */
TEST(left_paren)
{
    stringstream in("(");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_PAREN_LEFT == scanner.read());
    TEST_EXPECT(string("(") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A right paren can be read.
 */
TEST(right_paren)
{
    stringstream in(")");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_PAREN_RIGHT == scanner.read());
    TEST_EXPECT(string(")") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A dot can be read.
 */
TEST(dot)
{
    stringstream in(".");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_DOT == scanner.read());
    TEST_EXPECT(string(".") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A dot preceding digits can be read as a number.
 */
TEST(dot_number)
{
    const string VALUE = ".345";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A digit is a number.
 */
TEST(digit)
{
    const string VALUE = "7";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * Two digits with dots between them represent a number.
 */
TEST(digit_dot_digit)
{
    const string VALUE = "0.7";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A basic number can be read.
 */
TEST(number_basic)
{
    const string VALUE = "110.741";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A negative number can be read.
 */
TEST(number_negative)
{
    const string VALUE = "-17";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A number can have an exponent part.
 */
TEST(number_exponent)
{
    const string VALUE = "10e4";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A number can have a negative exponent part.
 */
TEST(number_negative_exponent)
{
    const string VALUE = "10e-4";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A number can have a capital exponent part.
 */
TEST(number_capital_exponent)
{
    const string VALUE = "10E4";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A number can have a negative capital exponent part.
 */
TEST(number_capital_negative_exponent)
{
    const string VALUE = "10E-4";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * An exponent must have a digit after it.
 */
TEST(exponent_digit)
{
    const string VALUE = "10e";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_INVALID == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
}

/**
 * A negative exponent must have a digit after it.
 */
TEST(exponent_negative_digit)
{
    const string VALUE = "10e-";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_INVALID == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
}

/**
 * A minus by itself is not valid.
 */
TEST(minus)
{
    const string VALUE = "-";
    stringstream in(VALUE);
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_INVALID == scanner.read());
    TEST_EXPECT(VALUE == scanner.get_token_string());
}

/**
 * An equals token can be read.
 */
TEST(equals)
{
    stringstream in("=");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_EQUALS == scanner.read());
    TEST_EXPECT(string("=") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * A number and an equals can both be read.
 */
TEST(number_equals)
{
    auto in = make_shared<stringstream>("7=");
    auto scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner->read());
    TEST_EXPECT(string("7") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EQUALS == scanner->read());
    TEST_EXPECT(string("=") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("7.0=");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner->read());
    TEST_EXPECT(string("7.0") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EQUALS == scanner->read());
    TEST_EXPECT(string("=") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());

    in = make_shared<stringstream>("7e0=");
    scanner = make_shared<lexer>(*in);

    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner->read());
    TEST_EXPECT(string("7e0") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EQUALS == scanner->read());
    TEST_EXPECT(string("=") == scanner->get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner->read());
}

/**
 * after keyword.
 */
TEST(keyword_after)
{
    stringstream in("after");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_AFTER == scanner.read());
    TEST_EXPECT(string("after") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * a is an identifier.
 */
TEST(identifier_a)
{
    stringstream in("a");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("a") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * a is an identifier.
 */
TEST(identifier_a_space)
{
    stringstream in("a ");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("a") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * am is an identifier.
 */
TEST(identifier_am)
{
    stringstream in("am");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("am") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * af is an identifier.
 */
TEST(identifier_af)
{
    stringstream in("af");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("af") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * aft is an identifier.
 */
TEST(identifier_aft)
{
    stringstream in("aft");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("aft") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * afte is an identifier.
 */
TEST(identifier_afte)
{
    stringstream in("afte");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("afte") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * afterward identifier.
 */
TEST(identifier_afterward)
{
    stringstream in("afterward");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("afterward") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * assert keyword.
 */
TEST(keyword_assert)
{
    stringstream in("assert");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_ASSERT == scanner.read());
    TEST_EXPECT(string("assert") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * as identifier.
 */
TEST(identifier_as)
{
    stringstream in("as");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("as") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * ass identifier.
 */
TEST(identifier_ass)
{
    stringstream in("ass");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ass") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * asse identifier.
 */
TEST(identifier_asse)
{
    stringstream in("asse");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("asse") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * asser identifier.
 */
TEST(identifier_asser)
{
    stringstream in("asser");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("asser") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * asserted identifier.
 */
TEST(identifier_asserted)
{
    stringstream in("asserted");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("asserted") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * at keyword.
 */
TEST(keyword_at)
{
    stringstream in("at");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_AT == scanner.read());
    TEST_EXPECT(string("at") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * ate identifier.
 */
TEST(identifier_ate)
{
    stringstream in("ate");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ate") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * component keyword.
 */
TEST(keyword_component)
{
    stringstream in("component");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_COMPONENT == scanner.read());
    TEST_EXPECT(string("component") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * c identifier.
 */
TEST(identifier_c)
{
    stringstream in("c");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("c") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * co identifier.
 */
TEST(identifier_co)
{
    stringstream in("co");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("co") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * com identifier.
 */
TEST(identifier_com)
{
    stringstream in("com");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("com") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * comp identifier.
 */
TEST(identifier_comp)
{
    stringstream in("comp");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("comp") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * compo identifier.
 */
TEST(identifier_compo)
{
    stringstream in("compo");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("compo") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * compon identifier.
 */
TEST(identifier_compon)
{
    stringstream in("compon");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("compon") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * compone identifier.
 */
TEST(identifier_compone)
{
    stringstream in("compone");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("compone") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * componen identifier.
 */
TEST(identifier_componen)
{
    stringstream in("componen");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("componen") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * components identifier.
 */
TEST(identifier_components)
{
    stringstream in("components");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("components") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * examine keyword.
 */
TEST(keyword_examine)
{
    stringstream in("examine");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_EXAMINE == scanner.read());
    TEST_EXPECT(string("examine") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * eu identifier.
 */
TEST(identifier_eu)
{
    stringstream in("eu");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("eu") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * ex identifier.
 */
TEST(identifier_ex)
{
    stringstream in("ex");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ex") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exa identifier.
 */
TEST(identifier_exa)
{
    stringstream in("exa");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exa") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exam identifier.
 */
TEST(identifier_exam)
{
    stringstream in("exam");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exam") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exami identifier.
 */
TEST(identifier_exami)
{
    stringstream in("exami");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exami") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * examin identifier.
 */
TEST(identifier_examin)
{
    stringstream in("examin");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("examin") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * examined identifier.
 */
TEST(identifier_examined)
{
    stringstream in("examined");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("examined") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * execution keyword.
 */
TEST(keyword_execution)
{
    stringstream in("execution");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_EXECUTION == scanner.read());
    TEST_EXPECT(string("execution") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exe identifier.
 */
TEST(identifier_exe)
{
    stringstream in("exe");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exe") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exec identifier.
 */
TEST(identifier_exec)
{
    stringstream in("exec");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exec") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * execu identifier.
 */
TEST(identifier_execu)
{
    stringstream in("execu");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("execu") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * execut identifier.
 */
TEST(identifier_execut)
{
    stringstream in("execut");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("execut") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * executi identifier.
 */
TEST(identifier_executi)
{
    stringstream in("executi");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("executi") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * executio identifier.
 */
TEST(identifier_executio)
{
    stringstream in("executio");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("executio") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * executioner identifier.
 */
TEST(identifier_executioner)
{
    stringstream in("executioner");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("executioner") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * expect keyword.
 */
TEST(keyword_expect)
{
    stringstream in("expect");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_EXPECT == scanner.read());
    TEST_EXPECT(string("expect") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * exp identifier.
 */
TEST(identifier_exp)
{
    stringstream in("exp");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("exp") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * expe identifier.
 */
TEST(identifier_expe)
{
    stringstream in("expe");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("expe") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * expec identifier.
 */
TEST(identifier_expec)
{
    stringstream in("expec");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("expec") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * expects identifier.
 */
TEST(identifier_expects)
{
    stringstream in("expects");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("expects") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * false keyword.
 */
TEST(keyword_false)
{
    stringstream in("false");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_FALSE == scanner.read());
    TEST_EXPECT(string("false") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * f identifier.
 */
TEST(identifier_f)
{
    stringstream in("f");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("f") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * fa identifier.
 */
TEST(identifier_fa)
{
    stringstream in("fa");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("fa") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * fal identifier.
 */
TEST(identifier_fal)
{
    stringstream in("fal");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("fal") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * fals identifier.
 */
TEST(identifier_fals)
{
    stringstream in("fals");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("fals") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * falses identifier.
 */
TEST(identifier_falses)
{
    stringstream in("falses");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("falses") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * module keyword.
 */
TEST(keyword_module)
{
    stringstream in("module");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_MODULE == scanner.read());
    TEST_EXPECT(string("module") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * m identifier.
 */
TEST(identifier_m)
{
    stringstream in("m");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("m") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * mo identifier.
 */
TEST(identifier_mo)
{
    stringstream in("mo");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("mo") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * mod identifier.
 */
TEST(identifier_mod)
{
    stringstream in("mod");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("mod") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * modu identifier.
 */
TEST(identifier_modu)
{
    stringstream in("modu");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("modu") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * modul identifier.
 */
TEST(identifier_modul)
{
    stringstream in("modul");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("modul") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * modules identifier.
 */
TEST(identifier_modules)
{
    stringstream in("modules");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("modules") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pin keyword.
 */
TEST(keyword_pin)
{
    stringstream in("pin");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_PIN == scanner.read());
    TEST_EXPECT(string("pin") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * p identifier.
 */
TEST(identifier_p)
{
    stringstream in("p");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("p") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pi identifier.
 */
TEST(identifier_pi)
{
    stringstream in("pi");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pi") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pins identifier.
 */
TEST(identifier_pins)
{
    stringstream in("pins");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pins") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * probe keyword.
 */
TEST(keyword_probe)
{
    stringstream in("probe");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_PROBE == scanner.read());
    TEST_EXPECT(string("probe") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pr identifier.
 */
TEST(identifier_pr)
{
    stringstream in("pr");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pr") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pro identifier.
 */
TEST(identifier_pro)
{
    stringstream in("pro");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pro") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * prob identifier.
 */
TEST(identifier_prob)
{
    stringstream in("prob");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("prob") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * probed identifier.
 */
TEST(identifier_probed)
{
    stringstream in("probed");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("probed") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pulldown keyword.
 */
TEST(keyword_pulldown)
{
    stringstream in("pulldown");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_PULLDOWN == scanner.read());
    TEST_EXPECT(string("pulldown") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pu identifier.
 */
TEST(identifier_pu)
{
    stringstream in("pu");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pu") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pul identifier.
 */
TEST(identifier_pul)
{
    stringstream in("pul");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pul") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pull identifier.
 */
TEST(identifier_pull)
{
    stringstream in("pull");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pull") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pulld identifier.
 */
TEST(identifier_pulld)
{
    stringstream in("pulld");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pulld") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pulldo identifier.
 */
TEST(identifier_pulldo)
{
    stringstream in("pulldo");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pulldo") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pulldow identifier.
 */
TEST(identifier_pulldow)
{
    stringstream in("pulldow");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pulldow") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pulldowns identifier.
 */
TEST(identifier_pulldowns)
{
    stringstream in("pulldowns");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pulldowns") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pullup keyword.
 */
TEST(keyword_pullup)
{
    stringstream in("pullup");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_PULLUP == scanner.read());
    TEST_EXPECT(string("pullup") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pullu identifier.
 */
TEST(identifier_pullu)
{
    stringstream in("pullu");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pullu") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * pullups identifier.
 */
TEST(identifier_pullups)
{
    stringstream in("pullups");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("pullups") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scenario keyword.
 */
TEST(keyword_scenario)
{
    stringstream in("scenario");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_SCENARIO == scanner.read());
    TEST_EXPECT(string("scenario") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * s identifier.
 */
TEST(identifier_s)
{
    stringstream in("s");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("s") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * sc identifier.
 */
TEST(identifier_sc)
{
    stringstream in("sc");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("sc") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * sce identifier.
 */
TEST(identifier_sce)
{
    stringstream in("sce");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("sce") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scen identifier.
 */
TEST(identifier_scen)
{
    stringstream in("scen");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("scen") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scena identifier.
 */
TEST(identifier_scena)
{
    stringstream in("scena");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("scena") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scenar identifier.
 */
TEST(identifier_scenar)
{
    stringstream in("scenar");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("scenar") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scenari identifier.
 */
TEST(identifier_scenari)
{
    stringstream in("scenari");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("scenari") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * scenarios identifier.
 */
TEST(identifier_scenarios)
{
    stringstream in("scenarios");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("scenarios") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * signal keyword.
 */
TEST(keyword_signal)
{
    stringstream in("signal");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_SIGNAL == scanner.read());
    TEST_EXPECT(string("signal") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * si identifier.
 */
TEST(keyword_si)
{
    stringstream in("si");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("si") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * sig identifier.
 */
TEST(keyword_sig)
{
    stringstream in("sig");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("sig") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * sign identifier.
 */
TEST(keyword_sign)
{
    stringstream in("sign");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("sign") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * signa identifier.
 */
TEST(keyword_signa)
{
    stringstream in("signa");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("signa") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * signals identifier.
 */
TEST(keyword_signals)
{
    stringstream in("signals");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("signals") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * start keyword.
 */
TEST(keyword_start)
{
    stringstream in("start");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_START == scanner.read());
    TEST_EXPECT(string("start") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * st identifier.
 */
TEST(identifier_st)
{
    stringstream in("st");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("st") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * sta identifier.
 */
TEST(identifier_sta)
{
    stringstream in("sta");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("sta") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * star identifier.
 */
TEST(identifier_star)
{
    stringstream in("star");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("star") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * started identifier.
 */
TEST(identifier_started)
{
    stringstream in("started");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("started") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * state keyword.
 */
TEST(keyword_state)
{
    stringstream in("state");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_STATE == scanner.read());
    TEST_EXPECT(string("state") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * stat identifier.
 */
TEST(identifier_stat)
{
    stringstream in("stat");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("stat") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * stated identifier.
 */
TEST(identifier_stated)
{
    stringstream in("stated");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("stated") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * true keyword.
 */
TEST(keyword_true)
{
    stringstream in("true");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_TRUE == scanner.read());
    TEST_EXPECT(string("true") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * t identifier.
 */
TEST(identifier_t)
{
    stringstream in("t");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("t") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * tr identifier.
 */
TEST(identifier_tr)
{
    stringstream in("tr");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("tr") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * tru identifier.
 */
TEST(identifier_tru)
{
    stringstream in("tru");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("tru") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * trued identifier.
 */
TEST(identifier_trued)
{
    stringstream in("trued");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("trued") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * type keyword.
 */
TEST(keyword_type)
{
    stringstream in("type");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_TYPE == scanner.read());
    TEST_EXPECT(string("type") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * ty identifier.
 */
TEST(identifier_ty)
{
    stringstream in("ty");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ty") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * typ identifier.
 */
TEST(identifier_typ)
{
    stringstream in("typ");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("typ") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * typed identifier.
 */
TEST(identifier_typed)
{
    stringstream in("typed");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("typed") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * wire keyword.
 */
TEST(keyword_wire)
{
    stringstream in("wire");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_WIRE == scanner.read());
    TEST_EXPECT(string("wire") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * w identifier.
 */
TEST(identifier_w)
{
    stringstream in("w");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("w") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * wi identifier.
 */
TEST(identifier_wi)
{
    stringstream in("wi");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("wi") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * wir identifier.
 */
TEST(identifier_wir)
{
    stringstream in("wir");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("wir") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * wired identifier.
 */
TEST(identifier_wired)
{
    stringstream in("wired");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("wired") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * string token.
 */
TEST(token_string)
{
    stringstream in("\"xyzzy\"");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_STRING == scanner.read());
    TEST_EXPECT(string("\"xyzzy\"") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_EOF == scanner.read());
}

/**
 * Component lexing.
 */
TEST(component_example)
{
    stringstream in(
        R"TEST(
            component u1 {
                type ic7408
                config["propagation_delay"] := ns(27)
            }
        )TEST");
    lexer scanner(in);

    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_COMPONENT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("u1") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACE_LEFT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_TYPE == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ic7408") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("config") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_LEFT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_STRING == scanner.read());
    TEST_EXPECT(string("\"propagation_delay\"") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_BRACKET_RIGHT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_ASSIGN == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_IDENTIFIER == scanner.read());
    TEST_EXPECT(string("ns") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_PAREN_LEFT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == scanner.read());
    TEST_EXPECT(string("27") == scanner.get_token_string());
    TEST_EXPECT(HOMESIM_TOKEN_PAREN_RIGHT == scanner.read());
    TEST_EXPECT(HOMESIM_TOKEN_BRACE_RIGHT == scanner.read());
}
