/**
 * \file logic/component.cpp
 *
 * \brief Component constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Constructor for component.
 *
 * \param pins      Initializer list of pins.
 */
homesim::component::component(initializer_list<string> pins)
    : pin_name_array(pins)
{
    for (auto pn : pin_name_array)
    {
        pin_map.insert(make_pair(pn, nullptr));
    }
}
