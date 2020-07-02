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

void homesim::semantic_analyzer::analyze_wires()
{
    /* Go through each wire, building up a proper wire map. */
    for (auto i : module->wire_map)
    {
        /* verify that a wire by this name does not already exist. */
        auto wf = wire_map.find(i.first);
        if (wire_map.end() != wf)
            throw semantic_error(
                string("Duplicate definition for wire ") + i.first + " found.");

        /* create a wire. */
        auto w = make_shared<wire>();

        /* add the wire to our wire map. */
        wire_map.insert(make_pair(i.first, w));

        /* go through each connection in the wire. */
        for (auto j : i.second->connection_list)
        {
            /* look up the component. */
            auto f = component_map.find(j->component);
            if (component_map.end() == f)
                throw semantic_error(
                    string("In wire ") + i.first
                    + ": reference to unknown component " + j->component + ".");

            /* attempt to set the pin with this wire. */
            try
            {
                f->second->set_pin(j->pin, w.get());
            }
            catch (invalid_pin_error& e)
            {
                throw semantic_error(
                    string("In wire ") + i.first
                  + ": reference to unknown pin " + j->component
                  + ".pin[\"" + j->pin + "\"].");
            }
            catch (pin_binding_error& e)
            {
                throw semantic_error(
                    string("In wire ") + i.first
                  + ": pin " + j->component
                  + ".pin[\"" + j->pin + "\"] is already bound.");
            }
        }

        /* TODO - add pullup / pulldown. */

        /* TODO - flag if the pin is exported. */

        /* TODO - flag if there is an external input source. */
    }
}
