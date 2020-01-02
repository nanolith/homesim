/**
 * \file logic/wire_get_pull_downs.cpp
 *
 * \brief Get the number of pull-downs on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of pull-downs associated with this wire.
 *
 * \brief return the number of pull-downs.
 */
int homesim::wire::get_pull_downs() const
{
    return pull_downs;
}
