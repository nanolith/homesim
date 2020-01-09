/**
 * \file parser/parser_read.cpp
 *
 * \brief Read a token pair from either the stream or the lookahead.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <sstream>

using namespace homesim;
using namespace std;

token_pair
homesim::parser::read()
{
    token_pair ret;

    if (lookahead.size() > 0)
    {
        ret = lookahead.front();
        lookahead.pop_front();
    }
    else
    {
        ret.first = in.read();
        if (HOMESIM_TOKEN_EOF != ret.first)
            ret.second = in.get_token_string();
    }

    return ret;
}
