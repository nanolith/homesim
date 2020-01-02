/**
 * \file logic/wire_get_inputs.cpp
 *
 * \brief Get the number of input connections on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of input connections associated with this wire.
 *
 * \brief return the number of inputs.
 */
int homesim::wire::get_inputs() const
{
    return inputs;
}
