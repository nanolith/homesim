/**
 * \file platform/environment.cpp
 *
 * \brief Environment constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <homesim/environment.h>

using namespace homesim;
using namespace std;

/**
 * \brief Construct an empty environment.
 */
homesim::environment::environment()
    : cf(make_shared<component_factory>())
{
}
