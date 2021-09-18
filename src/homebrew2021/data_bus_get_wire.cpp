/**
 * \file data_bus_get_wire.cpp
 *
 * \brief Get a wire from the data bus.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include <sstream>

#include "data_bus.h"

using namespace homesim;
using namespace std;

wire* homebrew2021::data_bus::get_wire(int offset)
{
    if (offset < 0 || offset >= 8)
    {
        stringstream sout;

        sout << __FILE__ << ":" << __LINE__ << ": out of bounds with "
             << offset;
        throw bounds_error(sout.str());
    }

    return bus[offset].get();
}
