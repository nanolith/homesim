/**
 * \file homesim/ic/7404.h
 *
 * \brief Declarations for a 7404-like (Hex Inverter Gate) IC.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7404_HEADER_GUARD
# define HOMESIM_IC_7404_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/inverter.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7404 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS04.
 */
constexpr double ic7404_delay = 22.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7404 simulates a 7404 (Hex Inverter Gate) IC.
 */
class ic7404
{
public:

    /**
     * \brief ic7404 constructor, with parameters in roughly DIP package order.
     *
     * \param in1           Input for gate 1.
     * \param out1          Output for gate 1.
     * \param in2           Input for gate 2.
     * \param out2          Output for gate 2.
     * \param in3           Input for gate 3.
     * \param out3          Output for gate 3.
     * \param out4          Output for gate 4.
     * \param in4           Input for gate 4.
     * \param out5          Output for gate 5.
     * \param in5           Input for gate 5.
     * \param out6          Output for gate 6.
     * \param in6           Input for gate 6.
     * \param delay         The optional delay in seconds.
     */
    ic7404(
        wire* in1, wire* out1, wire* in2, wire* out2, wire* in3, wire* out3,
        wire* out4, wire* in4, wire* out5, wire* in5, wire* out6, wire* in6,
        double delay = ic7404_delay);

private:

    inverter g1;
    inverter g2;
    inverter g3;
    inverter g4;
    inverter g5;
    inverter g6;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7404_HEADER_GUARD*/
