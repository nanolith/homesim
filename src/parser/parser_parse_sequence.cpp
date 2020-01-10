/**
 * \file parser/parser_parse_sequence.cpp
 *
 * \brief Parse a sequence of tokens.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <sstream>

using namespace homesim;
using namespace std;

void
homesim::parser::parse_sequence(
    initializer_list<
        pair<token, shared_ptr<function<void (const token_pair&)>>>> seq)
{
    stringstream sout;

    for (auto i : seq)
    {
        token_pair t = read();

        /* fail if not the right token. */
        if (i.first != t.first)
        {
            int sl, sc, el, ec;
            in.read_linecol(sl, sc, el, ec);

            sout << "Error at " << sl << ":" << sc << ": ";
            sout << "Expecting " << token_to_description(i.first)
                 << ". Got " << token_to_description(t.first) << ".";

            throw parser_error(sout.str());
        }

        /* otherwise, run the lambda expression if valid. */
        if (!!i.second)
            (*i.second)(t);
    }
}
