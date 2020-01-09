/**
 * \file parser/parser.cpp
 *
 * \brief Parser constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

/**
 * \brief Create a parser instance backed by the given input stream.
 *
 * \param input         The input stream for this instance.
 */
homesim::parser::parser(std::istream& input)
    : in(input)
{
}
