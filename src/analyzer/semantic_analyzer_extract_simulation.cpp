/**
 * \file analyzer/semantic_analyzer_extract_simulation.cpp
 *
 * \brief Extract a simulation from the analyzer.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/semantic_analyzer.h>
#include <homesim/simulation.h>

using namespace homesim;
using namespace std;

/**
 * \brief Extract a simulation from the analyzer.
 *
 * \returns a simulation on success.
 *
 * \throws a \ref semantic_analysis_error on failure.
 */
shared_ptr<simulation> homesim::semantic_analyzer::extract_simulation()
{
    auto retval = make_shared<simulation>();

    /* run analysis. */
    analyze();

    return retval;
}
