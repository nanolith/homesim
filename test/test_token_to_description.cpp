/**
 * \file test/test_token_to_description.cpp
 *
 * \brief Unit tests for token_to_description.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <memory>
#include <minunit/minunit.h>
#include <sstream>

using namespace homesim;
using namespace std;

TEST_SUITE(token_to_description);

TEST(assign)
{
    TEST_EXPECT(
        string(":=") == token_to_description(HOMESIM_TOKEN_ASSIGN));
}

TEST(asterisk)
{
    TEST_EXPECT(
        string("*") == token_to_description(HOMESIM_TOKEN_ASTERISK));
}

TEST(brace_left)
{
    TEST_EXPECT(
        string("{") == token_to_description(HOMESIM_TOKEN_BRACE_LEFT));
}

TEST(brace_right)
{
    TEST_EXPECT(
        string("}") == token_to_description(HOMESIM_TOKEN_BRACE_RIGHT));
}

TEST(bracket_left)
{
    TEST_EXPECT(
        string("[") == token_to_description(HOMESIM_TOKEN_BRACKET_LEFT));
}

TEST(bracket_right)
{
    TEST_EXPECT(
        string("]") == token_to_description(HOMESIM_TOKEN_BRACKET_RIGHT));
}

TEST(dot)
{
    TEST_EXPECT(
        string(".") == token_to_description(HOMESIM_TOKEN_DOT));
}

TEST(equals)
{
    TEST_EXPECT(
        string("=") == token_to_description(HOMESIM_TOKEN_EQUALS));
}

TEST(identifier)
{
    TEST_EXPECT(
        string("identifier") == token_to_description(HOMESIM_TOKEN_IDENTIFIER));
}

TEST(keyword_after)
{
    TEST_EXPECT(
        string("after") == token_to_description(HOMESIM_TOKEN_KEYWORD_AFTER));
}

TEST(keyword_assert)
{
    TEST_EXPECT(
        string("assert") == token_to_description(HOMESIM_TOKEN_KEYWORD_ASSERT));
}

TEST(keyword_at)
{
    TEST_EXPECT(
        string("at") == token_to_description(HOMESIM_TOKEN_KEYWORD_AT));
}

TEST(keyword_component)
{
    TEST_EXPECT(
        string("component") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_COMPONENT));
}

TEST(keyword_examine)
{
    TEST_EXPECT(
        string("examine") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_EXAMINE));
}

TEST(keyword_execution)
{
    TEST_EXPECT(
        string("execution") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_EXECUTION));
}

TEST(keyword_expect)
{
    TEST_EXPECT(
        string("expect") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_EXPECT));
}

TEST(keyword_false)
{
    TEST_EXPECT(
        string("false") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_FALSE));
}

TEST(keyword_module)
{
    TEST_EXPECT(
        string("module") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_MODULE));
}

TEST(keyword_pin)
{
    TEST_EXPECT(
        string("pin") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_PIN));
}

TEST(keyword_probe)
{
    TEST_EXPECT(
        string("probe") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_PROBE));
}

TEST(keyword_pulldown)
{
    TEST_EXPECT(
        string("pulldown") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_PULLDOWN));
}

TEST(keyword_pullup)
{
    TEST_EXPECT(
        string("pullup") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_PULLUP));
}

TEST(keyword_scenario)
{
    TEST_EXPECT(
        string("scenario") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_SCENARIO));
}

TEST(keyword_signal)
{
    TEST_EXPECT(
        string("signal") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_SIGNAL));
}

TEST(keyword_start)
{
    TEST_EXPECT(
        string("start") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_START));
}

TEST(keyword_state)
{
    TEST_EXPECT(
        string("state") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_STATE));
}

TEST(keyword_true)
{
    TEST_EXPECT(
        string("true") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_TRUE));
}

TEST(keyword_type)
{
    TEST_EXPECT(
        string("type") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_TYPE));
}

TEST(keyword_wire)
{
    TEST_EXPECT(
        string("wire") ==
            token_to_description(HOMESIM_TOKEN_KEYWORD_WIRE));
}

TEST(number)
{
    TEST_EXPECT(
        string("number") ==
            token_to_description(HOMESIM_TOKEN_NUMBER));
}

TEST(paren_left)
{
    TEST_EXPECT(
        string("(") ==
            token_to_description(HOMESIM_TOKEN_PAREN_LEFT));
}

TEST(paren_right)
{
    TEST_EXPECT(
        string(")") ==
            token_to_description(HOMESIM_TOKEN_PAREN_RIGHT));
}

TEST(string)
{
    TEST_EXPECT(
        string("string") ==
            token_to_description(HOMESIM_TOKEN_STRING));
}

TEST(eof)
{
    TEST_EXPECT(
        string("EOF") ==
            token_to_description(HOMESIM_TOKEN_EOF));
}

TEST(invalid)
{
    TEST_EXPECT(
        string("(invalid)") ==
            token_to_description(HOMESIM_TOKEN_INVALID));
}
