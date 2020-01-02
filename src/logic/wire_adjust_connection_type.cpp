/**
 * \file logic/wire_adjust_connection_type.cpp
 *
 * \brief Perform an adjustment on a connection type counter.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Perform an adjustment on a connection type counter.
 *
 * \param type          The counter on which this adjustment is performed.
 * \param adjustment    The delta for the adjustment.
 */
void homesim::wire::adjust_connection_type(
    wire_connection_type type, int adjustment)
{
    switch (type)
    {
        case WIRE_CONNECTION_TYPE_INPUT:
            inputs += adjustment;
            break;

        case WIRE_CONNECTION_TYPE_OUTPUT:
            outputs += adjustment;
            break;

        case WIRE_CONNECTION_TYPE_PULL_DOWN:
            pull_downs += adjustment;
            break;

        case WIRE_CONNECTION_TYPE_PULL_UP:
            pull_ups += adjustment;
            break;

        case WIRE_CONNECTION_TYPE_HIGH_Z:
            high_zs += adjustment;
            break;
    }
}
