/**
 * \file homesim/ic/7400.h
 *
 * \brief Declarations for a 7400-like (Quad NAND Gate) IC.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_7400_HEADER_GUARD
# define HOMESIM_IC_7400_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/nand_gate.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 7402 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS00.
 */
constexpr double ic7400_delay = 22.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic7402 simulates a 7400 (Quad NAND Gate) IC.
 */
class ic7400
{
public:

    /**
     * \brief ic7400 constructor, with parameters in roughly DIP package order.
     *
     * \param in1a          Input A for gate 1.
     * \param in1b          Input B for gate 1.
     * \param out1y         Output of gate 1.
     * \param in2a          Input A for gate 2.
     * \param in2b          Input B for gate 2.
     * \param out2y         Output of gate 2.
     * \param out3y         Output for gate 3.
     * \param in3b          Input B for gate 3.
     * \param in3a          Input A for gate 3.
     * \param out4y         Output for gate 4.
     * \param in4b          Input B for gate 4.
     * \param in4a          Input A for gate 4.
     * \param delay         The optional delay in seconds.
     */
    ic7400(
        wire* in1a, wire* in1b, wire* out1y, wire* in2a, wire* in2b,
        wire* out2y, wire* out3y, wire* in3b, wire* in3a, wire* out4y,
        wire* in4b, wire* in4a, double delay = ic7400_delay);

private:
    nand_gate g1;
    nand_gate g2;
    nand_gate g3;
    nand_gate g4;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_7400_HEADER_GUARD*/
