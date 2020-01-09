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

shared_ptr<parser::error_list>
homesim::parser::parse_sequence(
    initializer_list<
        pair<token, shared_ptr<function<void (const token_pair&)>>>> seq)
{
    stringstream sout;
    auto errors = make_shared<error_list>();

    for (auto i : seq)
    {
        token_pair t = read();

        /* fail if not the right token. */
        if (i.first != t.first)
        {

            sout << "Expecting " << token_to_description(i.first)
                 << ". Got " << token_to_description(t.first) << ".";

            errors->push_back(sout.str());
            return errors;
        }

        /* otherwise, run the lambda expression if valid. */
        if (!!i.second)
            (*i.second)(t);
    }

    return nullptr;
}
