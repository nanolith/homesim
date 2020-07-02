/**
 * \file logic/component_pin_name.cpp
 *
 * \brief Get the name of a pin for a given index value.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the pin name for the given index.
 *
 * \param index     The index of the pin name to get.
 */
const string& homesim::component::pin_name(int index) const
{
    return pin_name_array[index];
}
