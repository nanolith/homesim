/**
 * \file logic/component_pins.cpp
 *
 * \brief Get the number of pins for the component.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of pins for this component.
 */
int homesim::component::pins() const
{
    return pin_name_array.size();
}
