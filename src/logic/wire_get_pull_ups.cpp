/**
 * \file logic/wire_get_pull_ups.cpp
 *
 * \brief Get the number of pull-ups on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of pull-ups associated with this wire.
 *
 * \brief return the number of pull-ups.
 */
int homesim::wire::get_pull_ups() const
{
    return pull_ups;
}
