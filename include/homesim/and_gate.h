/**
 * \file homesim/and_gate.h
 *
 * \brief Declarations for an and gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_AND_GATE_HEADER_GUARD
# define HOMESIM_AND_GATE_HEADER_GUARD

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
 * \brief And Gate delay.
 *
 * By default, the and gate delay is 1 nanosecond.
 */
constexpr double and_gate_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The and_gate simulates a gate that performs a logical and of its
 * inputs.
 */
class and_gate
{
public:

    /**
     * \brief And gate constructor.
     *
     * \param a1p       The first input for the and gate.
     * \param a2p       The second input for the and gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    and_gate(wire* a1p, wire* a2p, wire* outp, double delay = and_gate_delay);

private:
    wire* a1;
    wire* a2;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_AND_GATE_HEADER_GUARD*/
