/**
 * \file bus_register_get_data_wire.cpp
 *
 * \brief Get a data wire from the bus register.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#include "bus_register.h"

using namespace homesim;
using namespace std;

wire* homebrew2021::bus_register::get_data_wire(int offset)
{
    if (offset < 0 || offset > 7)
        throw bounds_error("data wire offset out of bounds.");

    return data_wires[offset].get();
}
