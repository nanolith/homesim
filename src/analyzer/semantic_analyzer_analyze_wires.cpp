/**
 * \file analyzer/semantic_analyzer_analyze.cpp
 *
 * \brief Analyze the wires of a configuration.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <homesim/environment.h>
#include <homesim/semantic_analyzer.h>
#include <homesim/simulation.h>

using namespace homesim;
using namespace std;

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
