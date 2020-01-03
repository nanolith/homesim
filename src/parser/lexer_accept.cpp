/**
 * \file parser/lexer_accept.cpp
 *
 * \brief Accept a character as part of the string representation of a token.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <istream>

using namespace homesim;
using namespace std;

void homesim::lexer::accept(int ch)
{
    tokenbuf.push_back(ch);
}
