/**
 * \file homesim/ic/7432.h
 *
 * \brief Declarations for a 7432-like (Quad OR Gate) IC.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7432_HEADER_GUARD
# define HOMESIM_IC_7432_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/or_gate.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7432 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS32.
 */
constexpr double ic7432_delay = 22.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7432 simulates a 7402 (Quad OR Gate) IC.
 */
class ic7432
{
public:

    /**
     * \brief ic7432 constructor, with parameters in roughly DIP package order.
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
    ic7432(
        wire* in1a, wire* in1b, wire* out1y, wire* in2a, wire* in2b,
        wire* out2y, wire* out3y, wire* in3a, wire* in3b, wire* out4y,
        wire* in4a, wire* in4b, double delay = ic7432_delay);

private:

    or_gate g1;
    or_gate g2;
    or_gate g3;
    or_gate g4;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7432_HEADER_GUARD*/
