/**
 * \file logic/wire.cpp
 *
 * \brief Add an action to occur when the signal on this wire changes.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Add an action to occur when the wire signal changes.
 *
 * \param action        The action to perform on signal change.
 */
void homesim::wire::add_action(function<void ()> action)
{
    actions.push_back(action);

    action();
}
