/**
 * \file logic/agenda_next.cpp
 *
 * \brief Get the next item on the agenda.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the next action to be performed according to the simulation
 * schedule.
 *
 * \returns a pair of bool and function.
 *      - if the Boolean value is true, this is a valid action.
 *      - if the Boolean value is false, there are no more actions on the
 *        agenda.
*/
pair<bool, function<void ()>> homesim::agenda::next()
{
    /* if the priority queue is empty, there are no new agenda items.*/
    if (queue.empty())
        return make_pair(false, []() { } );

    /* otherwise, return the next agenda item. */
    return
        make_pair(true, queue.top().second);
}
