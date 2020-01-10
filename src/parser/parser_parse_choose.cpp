/**
 * \file parser/parser_parse_choose.cpp
 *
 * \brief Parse a choice of tokens.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <sstream>

using namespace homesim;
using namespace std;

void
homesim::parser::parse_choose(
    initializer_list<
        pair<token, shared_ptr<function<void (const token_pair&)>>>> choices)
{
    typedef
    map<token, shared_ptr<function<void(const token_pair&)>>>
    choice_map_t;

    stringstream sout;
    choice_map_t choice_map(choices.begin(), choices.end());

    token_pair t = read();

    choice_map_t::iterator f = choice_map.find(t.first);
    if (choice_map.end() == f)
    {
        int sl, sc, el, ec;
        in.read_linecol(sl, sc, el, ec);
        sout << "Error at " << sl << ":" << sc << ": ";
        sout << "Expecting one of (";
        for (auto i : choice_map)
        {
            sout << " " << token_to_description(i.first) << " ";
        }
        sout << "). Got " << token_to_description(t.first) << ".";

        throw parser_error(sout.str());
    }

    if (!!f->second)
        (*f->second)(t);
}
