/**
 * \file logic/agenda_current_time.cpp
 *
 * \brief Get the agenda's current time in seconds.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the current time.
 *
 * \returns the current time in seconds.
 */
double homesim::agenda::current_time() const
{
    return time;
}
