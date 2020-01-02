/**
 * \file logic/wire.cpp
 *
 * \brief Constructor for wire.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Constructor for a wire.
 *
 * This constructor sets the wire to the default state, which is logical
 * false, no connections, and DRC checks (faults and floating) disabled.
 */
homesim::wire::wire()
    : signal(0)
    , floating(false)
    , fault(false)
    , inputs(0)
    , outputs(0)
    , high_zs(0)
    , pull_downs(0)
    , pull_ups(0)
{
}
