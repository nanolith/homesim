/**
 * \file homesim/xor_gate.h
 *
 * \brief Declarations for an xor gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_XOR_GATE_HEADER_GUARD
# define HOMESIM_XOR_GATE_HEADER_GUARD

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
 * \brief Xor Gate delay.
 *
 * By default, the xor gate delay is 1 nanosecond.
 */
constexpr double xor_gate_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The xor_gate simulates a gate that performs a logical exclusive or of
 * its inputs.
 */
class xor_gate
{
public:

    /**
     * \brief Xor gate constructor.
     *
     * \param x1p       The first input for the or gate.
     * \param x2p       The second input for the or gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    xor_gate(wire* x1p, wire* x2p, wire* outp, double delay = xor_gate_delay);

private:
    wire* x1;
    wire* x2;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_XOR_GATE_HEADER_GUARD*/
