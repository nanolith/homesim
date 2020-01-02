/**
 * \file homesim/buffer.h
 *
 * \brief Declarations for a buffer gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_BUFFER_HEADER_GUARD
# define HOMESIM_BUFFER_HEADER_GUARD

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
 * \brief Buffer delay.
 *
 * By default, the buffer delay is 1 nanosecond.
 */
constexpr double buffer_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The inverter simulates a gate that performs the identity operation on
 * its input.
 */
class buffer
{
public:

    /**
     * \brief Buffer constructor.
     *
     * \param inp       The input wire for this gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    buffer(wire* inp, wire* outp, double delay = buffer_delay);

private:
    wire* in;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_BUFFER_HEADER_GUARD*/
