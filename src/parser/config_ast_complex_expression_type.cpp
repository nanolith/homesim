/**
 * \file parser/config_ast_complex_expression_type.cpp
 *
 * \brief Get the type of a complex expression.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

token homesim::config_ast_complex_expression::type()
{
    /* TODO - for now, all complex expressions return a number. */
    return HOMESIM_TOKEN_NUMBER;
}
