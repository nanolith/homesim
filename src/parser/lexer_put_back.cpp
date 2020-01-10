/**
 * \file parser/lexer_put_back.cpp
 *
 * \brief Put a character back for later reading.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <istream>

using namespace homesim;
using namespace std;

void homesim::lexer::put_back(int ch)
{
    putbackbuf.push_front(ch);
}
