/**
 * \file parser/lexer_get_token_string.cpp
 *
 * \brief Get the string representation of a token.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <istream>

using namespace homesim;
using namespace std;

/**
 * \brief Get the string value of the current token.
 *
 * \returns string value of the token or an empty string if inappropriate.
 */
string homesim::lexer::get_token_string()
{
    return string(tokenbuf.begin(), tokenbuf.end());
}
