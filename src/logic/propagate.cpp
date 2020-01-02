/**
 * \file logic/propagate.cpp
 *
 * \brief Propagate all actions in the global agenda.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Propagate all outstanding actions in the simulation until the
 * simulation has converged.
 */
void homesim::propagate()
{
    for (;;)
    {
        /* get the next agenda item. */
        auto a = global_agenda.next();

        /* if there are no more agenda items, return. */
        if (!a.first)
            return;

        /* pop this agenda item, updating the current time. */
        global_agenda.pop();

        /* execute this agenda item. */
        a.second();
    }
}
