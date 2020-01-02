/**
 * \file homesim/xnor_gate.h
 *
 * \brief Declarations for an xnor gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_XNOR_GATE_HEADER_GUARD
# define HOMESIM_XNOR_GATE_HEADER_GUARD

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
 * \brief Xnor Gate delay.
 *
 * By default, the xnor gate delay is 1 nanosecond.
 */
constexpr double xnor_gate_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The xnor_gate simulates a gate that performs a logical exclusive nor
 * of its inputs.
 */
class xnor_gate
{
public:

    /**
     * \brief Xnor gate constructor.
     *
     * \param x1p       The first input for the nor gate.
     * \param x2p       The second input for the nor gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    xnor_gate(wire* x1p, wire* x2p, wire* outp, double delay = xnor_gate_delay);

private:
    wire* x1;
    wire* x2;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_XNOR_GATE_HEADER_GUARD*/
