/**
 * \file logic/wire_get_high_zs.cpp
 *
 * \brief Get the number of high-Z connections on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of high-Z connections associated with this wire.
 *
 * \brief return the number of high-Z connections.
 */
int homesim::wire::get_high_zs() const
{
    return high_zs;
}
