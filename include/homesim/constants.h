/**
 * \file homesim/constants.h
 *
 * \brief Constants used by the simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */

#ifndef  HOMESIM_CONSTANTS_HEADER_GUARD
# define HOMESIM_CONSTANTS_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

namespace homesim {

/**
 * \brief The scaling factor needed to convert nanoseconds to seconds.
 */
constexpr double nanoseconds_to_seconds_scale = 1.0 / 1000000000.0;

} /* namespace homesim */

#endif /*HOMESIM_CONSTANTS_HEADER_GUARD*/
