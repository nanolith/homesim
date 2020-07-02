/**
 * \file analyzer/semantic_analyzer.cpp
 *
 * \brief Semantic Analyzer constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/semantic_analyzer.h>

using namespace homesim;
using namespace std;

/**
 * \brief Semantic analyzer for homesim AST.
 *
 * \param en                The environment for this analysis.
 * \param mod               The module to analyze.
 */
homesim::semantic_analyzer::semantic_analyzer(
    std::shared_ptr<environment> en,
    std::shared_ptr<config_ast_module> mod)
        : env(en)
        , module(mod)
        , analyzed(false)
{
}
