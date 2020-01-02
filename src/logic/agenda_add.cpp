/**
 * \file logic/agenda_add.cpp
 *
 * \brief Add an action to the agenda.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Add an action to the agenda, to occur after the given delay.
 *
 * \param delay         The delay in seconds.
 * \param action        The action to occur.
 */
void homesim::agenda::add(double delay, function<void ()> action)
{
    queue.emplace(time + delay, action);
}
