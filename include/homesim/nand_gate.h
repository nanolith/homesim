/**
 * \file homesim/nand_gate.h
 *
 * \brief Declarations for a nand gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_NAND_GATE_HEADER_GUARD
# define HOMESIM_NAND_GATE_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/agenda.h>
#include <homesim/constants.h>
#include <homesim/wire.h>
#include <memory>

namespace homesim {

/**
 * \brief Nand Gate delay.
 *
 * By default, the nand gate delay is 1 nanosecond.
 */
constexpr double nand_gate_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The nand_gate simulates a gate that performs a logical nand of its
 * inputs.
 */
class nand_gate
{
public:

    /**
     * \brief Nand gate constructor.
     *
     * \param a1p       The first input for the nand gate.
     * \param a2p       The second input for the nand gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    nand_gate(wire* a1p, wire* a2p, wire* outp, double delay = nand_gate_delay);

private:
    wire* a1;
    wire* a2;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_NAND_GATE_HEADER_GUARD*/
