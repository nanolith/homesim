/**
 * \file logic/wire_add_connection.cpp
 *
 * \brief Add a connection to the wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Add a connection of the given type, enabling DRC checks.
 *
 * This type is used to adjust counters which are used when checking for a
 * floating signal or a DRC fault. Furthermore, it runs a set of state
 * change actions which can be used for probing DRC checks either at
 * configuration time or at runtime for dynamic circuits.
 *
 * \param type      The connection type to be added.
 */
void homesim::wire::add_connection(wire_connection_type type)
{
    adjust_connection_type(type, +1);

    fault_check();

    for (auto a : state_change_actions)
        a();
}
