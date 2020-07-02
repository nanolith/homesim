/**
 * \file logic/component_factory_register_component.cpp
 *
 * \brief Register a component in this component factory.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>

using namespace homesim;
using namespace std;

/**
 * \brief Register a component constructor by type.
 *
 * \param type      The type of the component to register.
 * \param ctor      The component constructor function to register.
 */
void homesim::component_factory::register_component(
    const string& type,
    function<shared_ptr<component> ()> ctor)
{
    component_map.insert(make_pair(type, ctor));
}
