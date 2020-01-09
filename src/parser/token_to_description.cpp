/**
 * \file parser/token_to_description.cpp
 *
 * \brief Convert a token to a string description.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

/**
 * Simple utility function to transform a lexer token into a string description.
 *
 * \param t         The token to convert to a string.
 *
 * \returns a string description of the token.
 */
string homesim::token_to_description(token t)
{
    switch (t)
    {
        case HOMESIM_TOKEN_ASSIGN:                  return ":=";
        case HOMESIM_TOKEN_ASTERISK:                return "*";
        case HOMESIM_TOKEN_BRACE_LEFT:              return "{";
        case HOMESIM_TOKEN_BRACE_RIGHT:             return "}";
        case HOMESIM_TOKEN_BRACKET_LEFT:            return "[";
        case HOMESIM_TOKEN_BRACKET_RIGHT:           return "]";
        case HOMESIM_TOKEN_DOT:                     return ".";
        case HOMESIM_TOKEN_EQUALS:                  return "=";
        case HOMESIM_TOKEN_IDENTIFIER:              return "identifier";
        case HOMESIM_TOKEN_KEYWORD_AFTER:           return "after";
        case HOMESIM_TOKEN_KEYWORD_ASSERT:          return "assert";
        case HOMESIM_TOKEN_KEYWORD_AT:              return "at";
        case HOMESIM_TOKEN_KEYWORD_COMPONENT:       return "component";
        case HOMESIM_TOKEN_KEYWORD_EXAMINE:         return "examine";
        case HOMESIM_TOKEN_KEYWORD_EXECUTION:       return "execution";
        case HOMESIM_TOKEN_KEYWORD_EXPECT:          return "expect";
        case HOMESIM_TOKEN_KEYWORD_FALSE:           return "false";
        case HOMESIM_TOKEN_KEYWORD_MODULE:          return "module";
        case HOMESIM_TOKEN_KEYWORD_PIN:             return "pin";
        case HOMESIM_TOKEN_KEYWORD_PROBE:           return "probe";
        case HOMESIM_TOKEN_KEYWORD_PULLDOWN:        return "pulldown";
        case HOMESIM_TOKEN_KEYWORD_PULLUP:          return "pullup";
        case HOMESIM_TOKEN_KEYWORD_SCENARIO:        return "scenario";
        case HOMESIM_TOKEN_KEYWORD_SIGNAL:          return "signal";
        case HOMESIM_TOKEN_KEYWORD_START:           return "start";
        case HOMESIM_TOKEN_KEYWORD_STATE:           return "state";
        case HOMESIM_TOKEN_KEYWORD_TRUE:            return "true";
        case HOMESIM_TOKEN_KEYWORD_TYPE:            return "type";
        case HOMESIM_TOKEN_KEYWORD_WIRE:            return "wire";
        case HOMESIM_TOKEN_NUMBER:                  return "number";
        case HOMESIM_TOKEN_PAREN_LEFT:              return "(";
        case HOMESIM_TOKEN_PAREN_RIGHT:             return ")";
        case HOMESIM_TOKEN_STRING:                  return "string";
        case HOMESIM_TOKEN_EOF:                     return "EOF";
        case HOMESIM_TOKEN_INVALID:                 return "(invalid)";
        default:                                    return "unknown token";
    }
}

