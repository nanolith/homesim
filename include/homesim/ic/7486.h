/**
 * \file homesim/ic/7486.h
 *
 * \brief Declarations for a 7486-like (Quad XOR Gate) IC.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7486_HEADER_GUARD
# define HOMESIM_IC_7486_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/xor_gate.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7486 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS32.
 */
constexpr double ic7486_delay = 23.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7486 simulates a 7402 (Quad XOR Gate) IC.
 */
class ic7486
{
public:

    /**
     * \brief ic7486 constructor, with parameters in roughly DIP package order.
     *
     * \param in1a          Input A for gate 1.
     * \param in1b          Input B for gate 1.
     * \param out1y         Output of gate 1.
     * \param in2a          Input A for gate 2.
     * \param in2b          Input B for gate 2.
     * \param out2y         Output of gate 2.
     * \param out3y         Output for gate 3.
     * \param in3a          Input A for gate 3.
     * \param in3b          Input B for gate 3.
     * \param out4y         Output for gate 4.
     * \param in4a          Input A for gate 4.
     * \param in4b          Input B for gate 4.
     * \param delay         The optional delay in seconds.
     */
    ic7486(
        wire* in1a, wire* in1b, wire* out1y, wire* in2a, wire* in2b,
        wire* out2y, wire* out3y, wire* in3a, wire* in3b, wire* out4y,
        wire* in4a, wire* in4b, double delay = ic7486_delay);

private:

    xor_gate g1;
    xor_gate g2;
    xor_gate g3;
    xor_gate g4;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7486_HEADER_GUARD*/
