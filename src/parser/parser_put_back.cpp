/**
 * \file parser/parser_put_back.cpp
 *
 * \brief Put back a token pair into the lookahead.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <sstream>

using namespace homesim;
using namespace std;

void
homesim::parser::put_back(const token_pair& t)
{
    lookahead.push_front(t);
}
