/**
 * \file parser/lexer_read_linecol.cpp
 *
 * \brief Lexer constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

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
void homesim::lexer::read_linecol(
    int& sl, int& sc, int& el, int& ec) const
{
    sl = start_line;
    sc = start_col;
    el = start_line;
    ec = sc + tokenbuf.size() - 1;
}
