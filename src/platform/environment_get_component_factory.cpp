/**
 * \file platform/environment_get_component_factory.cpp
 *
 * \brief Getter for component factory instance.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/environment.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the component factory for this environment.
 */
component_factory& homesim::environment::get_component_factory()
{
    return *cf;
}
