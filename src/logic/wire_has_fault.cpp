/**
 * \file logic/wire_has_fault.cpp
 *
 * \brief Does this wire have a DRC fault?
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>

using namespace homesim;
using namespace std;

/**
 * \brief Does this wire have a DRC fault?
 */
bool homesim::wire::has_fault() const
{
    return fault;
}
