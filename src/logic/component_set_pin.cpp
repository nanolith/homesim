/**
 * \file logic/component_set_pin.cpp
 *
 * \brief Set the wire for a pin.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Set a pin to the given wire.
 *
 * \param name      The name of the pin to set.
 * \param w         The wire to set.
 *
 * \throws
 *      - \ref invalid_pin_error if the pin name is invalid.
 *      - \ref pin_binding_error if the pin is already bound.
 */
void homesim::component::set_pin(const string& name, wire* w)
{
    /* attempt to find the pin by name. */
    auto f = pin_map.find(name);
    if (f == pin_map.end())
        throw invalid_pin_error(string("Pin ") + name + " is invalid.");

    /* verify it is not already bound. */
    if (nullptr != f->second)
        throw pin_binding_error(string("Pin ") + name + " is already bound.");

    f->second = w;
}
