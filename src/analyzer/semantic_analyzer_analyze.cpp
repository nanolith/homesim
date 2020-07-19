/**
 * \file analyzer/semantic_analyzer_analyze.cpp
 *
 * \brief Perform semantic analysis on the AST.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <homesim/environment.h>
#include <homesim/semantic_analyzer.h>
#include <homesim/simulation.h>

using namespace homesim;
using namespace std;

/**
 * \brief Perform analysis on the AST, verifying all details.
 *
 * \throws a \ref semantic_error on failure.
 */
void homesim::semantic_analyzer::analyze()
{
    if (analyzed)
        return;

    /* the name should not be blank. */
    if (string("") == module->name)
        throw semantic_error("Invalid module name.");

    /* perform component analysis. */
    analyze_components();

    /* perform wire analysis. */
    analyze_wires();
}
