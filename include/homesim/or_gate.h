/**
 * \file homesim/or_gate.h
 *
 * \brief Declarations for a or gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_OR_GATE_HEADER_GUARD
# define HOMESIM_OR_GATE_HEADER_GUARD

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
 * \brief Or Gate delay.
 *
 * By default, the or gate delay is 1 nanosecond.
 */
constexpr double or_gate_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The or_gate simulates a gate that performs a logical or of its
 * inputs.
 */
class or_gate
{
public:

    /**
     * \brief Or gate constructor.
     *
     * \param o1p       The first input for the or gate.
     * \param o2p       The second input for the or gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    or_gate(wire* o1p, wire* o2p, wire* outp, double delay = or_gate_delay);

private:
    wire* o1;
    wire* o2;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_OR_GATE_HEADER_GUARD*/
