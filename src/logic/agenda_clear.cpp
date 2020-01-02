/**
 * \file logic/agenda_clear.cpp
 *
 * \brief Clear the agenda and reset the clock to 0.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Clear the agenda and reset the time to 0.
 */
void homesim::agenda::clear()
{
    time = 0.0;

    queue_type clear_queue;
    swap(queue, clear_queue);
}
