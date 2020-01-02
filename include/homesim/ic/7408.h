/**
 * \file homesim/ic/7408.h
 *
 * \brief Declarations for a 7402-like (Quad AND Gate) IC.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7408_HEADER_GUARD
# define HOMESIM_IC_7408_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/and_gate.h>
#include <homesim/constants.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7408 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS08.
 */
constexpr double ic7408_delay = 27.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7408 simulates a 7408 (Quad AND Gate) IC.
 */
class ic7408
{
public:

    /**
     * \brief ic7408 constructor, with parameters in roughly DIP package order.
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
    ic7408(
        wire* in1a, wire* in1b, wire* out1y, wire* in2a, wire* in2b,
        wire* out2y, wire* out3y, wire* in3a, wire* in3b, wire* out4y,
        wire* in4a, wire* in4b, double delay = ic7408_delay);

private:

    and_gate g1;
    and_gate g2;
    and_gate g3;
    and_gate g4;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7408_HEADER_GUARD*/
