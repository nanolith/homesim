/**
 * \file logic/wire_get_signal.cpp
 *
 * \brief Get the signal value for this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the current signal value for this wire.
 *
 * \returns the signal value, which is either true or false.
 */
bool homesim::wire::get_signal() const
{
    return signal;
}
