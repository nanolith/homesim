/**
 * \file analyzer/semantic_analyzer_analyze_components.cpp
 *
 * \brief Analyze the components.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <homesim/environment.h>
#include <homesim/semantic_analyzer.h>
#include <homesim/simulation.h>

using namespace homesim;
using namespace std;

void homesim::semantic_analyzer::analyze_components()
{
    /* Go through each component, building up a proper component map. */
    for (auto i : module->component_map)
    {
        /* verify that a component by this name does not already exist. */
        auto f = component_map.find(i.first);
        if (component_map.end() != f)
            throw semantic_error(
                string("Duplicate definition for component ") + i.first
                + " found.");

        /* verify that the component's type is specified. */
        if (!i.second->type)
            throw semantic_error(
                string("Component ") + i.first + " is missing a type.");

        try
        {
            /* look up the component from the environment by type. */
            auto component =
                env->get_component_factory().create(*i.second->type);

            /* add the component to our map. */
            component_map.insert(make_pair(i.first, component));
        }
        catch (unknown_component_error& e)
        {
            throw semantic_error(
                string("Component type ")
              + *i.second->type
              + " can't be found.");
        }
    }
}
