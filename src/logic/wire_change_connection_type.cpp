/**
 * \file logic/wire_change_connection_type.cpp
 *
 * \brief Change a connection's type.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Change an existing connection type to a different connection type.
 *
 * Certain components, such as transceiver ICs or microcontrollers, can
 * change their connection types on the fly based on simulated conditions.
 * When this occurs, the simulated component can call this function in order
 * to update the current wire state and to perform runtime DRC checking. If
 * the connection type changes to output, then the signal value is used to
 * determine whether the new signal for the wire is true or false.
 *
 * \param oldty     The old type for this connection.
 * \param newty     The new type for this connection.
 * \param signal    The signal to propagate if connection type is becoming
 *                  an output.
 */
void homesim::wire::change_connection_type(
    wire_connection_type oldty, wire_connection_type newty, bool signal)
{
    /* change the connection type by adjusting the counters. */
    adjust_connection_type(oldty, -1);
    adjust_connection_type(newty, +1);

    /* perform a fault check on this wire. */
    fault_check();

    /* if the connection is the only output, send a signal. */
    if (outputs == 1 && newty == WIRE_CONNECTION_TYPE_OUTPUT)
    {
        set_signal(signal);
    }
    /* if there are no longer any outputs, and this change was the cause, check
     * for pull-up / pull-downs. */
    else if (outputs == 0 && oldty == WIRE_CONNECTION_TYPE_OUTPUT)
    {
        if (pull_ups > 0 && pull_downs == 0)
        {
            set_signal(true);
        }
        else if (pull_downs > 0 && pull_ups == 0)
        {
            set_signal(false);
        }
    }

    /* notify any listeners of this state change. */
    for (auto a : state_change_actions)
        a();
}
