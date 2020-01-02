/**
 * \file logic/wire_fault_check.cpp
 *
 * \brief Perform a fault check on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Perform a fault check on this wire.
 */
void homesim::wire::fault_check()
{
    /* there can't be more than one active output. */
    if (outputs > 1)
    {
        fault = true;
    }
    /* there can't be more than one active pull-up / pull-down. */
    else if (pull_downs > 0 && pull_ups > 0)
    {
        fault = true;
    }
    /* there can't be more than one active pull-up / pull-down. */
    else if (pull_downs > 1)
    {
        fault = true;
    }
    /* there can't be more than one active pull-up / pull-down. */
    else if (pull_ups > 1)
    {
        fault = true;
    }
    /* no wiring fault detected. */
    else
    {
        fault = false;
    }

    /* if there are no outputs or pull-ups / pull-downs, then this wire's signal
     * is floating. */
    if (outputs == 0 && pull_downs == 0 && pull_ups == 0)
    {
        floating = true;
    }
    else
    {
        floating = false;
    }
}
