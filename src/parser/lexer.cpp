/**
 * \file parser/lexer.cpp
 *
 * \brief Lexer constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

/**
 * \brief Create a lexer instance backed by the given input stream.
 *
 * \param input         The input stream for this instance.
 */
homesim::lexer::lexer(std::istream& input)
    : in(input)
    , curline(1)
    , curcol(0)
    , start_line(0)
    , start_col(0)
    , end_line(0)
    , end_col(0)
{
}
