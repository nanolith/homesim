/**
 * \file parser/lexer_start.cpp
 *
 * \brief Start a new string representation of a token.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <istream>

using namespace homesim;
using namespace std;

void homesim::lexer::start(int ch)
{
    start_line = curline;
    start_col = curcol;

    tokenbuf.clear();

    accept(ch);
}
