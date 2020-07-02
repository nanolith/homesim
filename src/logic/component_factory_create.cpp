/**
 * \file logic/component_factory_create.cpp
 *
 * \brief Component factory creation method.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Create a component by type.
 *
 * \param type      The name of the component to create.
 *
 * \throws \ref unknown_component_error if the component type is unknown.
 */
shared_ptr<component> homesim::component_factory::create(const string& type)
{
    auto f = component_map.find(type);
    if (component_map.end() == f)
        throw unknown_component_error(type);

    return f->second();
}
