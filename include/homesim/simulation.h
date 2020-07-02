/**
 * \file homesim/simulation.h
 *
 * \brief A container holding the simulation.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_SIMULATION_HEADER_GUARD
# define HOMESIM_SIMULATION_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <homesim/parser.h>

namespace homesim {

/**
 * \brief A container that holds the simulation.
 */
class simulation
{
public:
    /**
     * \brief Default constructor for simulation.
     */
    simulation();
};

} /* namespace homesim */

#endif /*HOMESIM_SIMULATION_HEADER_GUARD*/
