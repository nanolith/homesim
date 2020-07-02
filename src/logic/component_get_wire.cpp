/**
 * \file logic/component_get_wire.cpp
 *
 * \brief Get the wire for a pin by name.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the wire for a pin.
 *
 * \param pin_name  The name of the pin to get.
 *
 * \throws \ref missing_wire_error if the wire is missing.
 */
wire* homesim::component::get_wire(const std::string& pin_name)
{
    auto f = pin_map.find(pin_name);
    if (pin_map.end() == f)
        throw invalid_pin_error(pin_name);

    if (nullptr == f->second)
        throw missing_wire_error(pin_name);

    return f->second;
}
