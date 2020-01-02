/**
 * \file logic/wire_set_signal.cpp
 *
 * \brief Set the signal for this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Set the signal value for this wire.
 *
 * If the new signal value differs, notify listeners that a change has
 * occurred so it can be propagated in the simulation.
 */
void homesim::wire::set_signal(bool newsignal)
{
    if (signal == newsignal)
        return;

    signal = newsignal;

    for (auto a : actions)
        a();
}
