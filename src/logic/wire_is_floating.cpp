/**
 * \file logic/wire_is_floating.cpp
 *
 * \brief Is this wire floating?
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Is this wire floating?
 */
bool homesim::wire::is_floating() const
{
    return floating;
}
