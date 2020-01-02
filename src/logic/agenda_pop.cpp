/**
 * \file logic/agenda_pop.cpp
 *
 * \brief Pop the next item off of the agenda queue.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Pop the top item off of the agenda queue.
 */
void homesim::agenda::pop()
{
    /* if the queue is empty, don't do anything. */
    if (queue.empty())
        return;

    /* get the item off of the top of the queue. */
    auto p = queue.top();
    /* update the time. */
    time = p.first;
    /* remove this item from the queue. */
    queue.pop();
}
