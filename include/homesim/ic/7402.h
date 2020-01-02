/**
 * \file homesim/ic/7402.h
 *
 * \brief Declarations for a 7402-like (Quad NOR Gate) IC.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7402_HEADER_GUARD
# define HOMESIM_IC_7402_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/nor_gate.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7402 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS02.
 */
constexpr double ic7402_delay = 22.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7402 simulates a 7402 (Quad NOR Gate) IC.
 */
class ic7402
{
public:

    /**
     * \brief ic7402 constructor, with parameters in roughly DIP package order.
     *
     * \param out1y         Output of gate 1.
     * \param in1a          Input A for gate 1.
     * \param in1b          Input B for gate 1.
     * \param out2y         Output of gate 2.
     * \param in2a          Input A for gate 2.
     * \param in2b          Input B for gate 2.
     * \param in3a          Input A for gate 3.
     * \param in3b          Input B for gate 3.
     * \param out3y         Output for gate 3.
     * \param in4a          Input A for gate 4.
     * \param in4b          Input B for gate 4.
     * \param out4y         Output for gate 4.
     * \param delay         The optional delay in seconds.
     */
    ic7402(
        wire* out1y, wire* in1a, wire* in1b, wire* out2y, wire* in2a,
        wire* in2b, wire* in3a, wire* in3b, wire* out3y, wire* in4a,
        wire* in4b, wire* out4y, double delay = ic7402_delay);

private:
    nor_gate g1;
    nor_gate g2;
    nor_gate g3;
    nor_gate g4;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7402_HEADER_GUARD*/
