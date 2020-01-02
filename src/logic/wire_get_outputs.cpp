/**
 * \file logic/wire_get_outputs.cpp
 *
 * \brief Get the number of output connections on this wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Get the number of output connections associated with this wire.
 *
 * \brief return the number of outputs.
 */
int homesim::wire::get_outputs() const
{
    return outputs;
}
