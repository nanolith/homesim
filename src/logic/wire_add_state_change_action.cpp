/**
 * \file logic/wire_add_state_change_action.cpp
 *
 * \brief Add a connection level state change action to this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Add an action to occur when the connection level state changes.
 *
 * \param action        The action to perform on state change.
 */
void homesim::wire::add_state_change_action(function<void ()> action)
{
    state_change_actions.push_back(action);

    action();
}
