/**
 * \file homesim/inverter.h
 *
 * \brief Declarations for an inverter gate simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_INVERTER_HEADER_GUARD
# define HOMESIM_INVERTER_HEADER_GUARD

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
 * \brief Inverter delay.
 *
 * By default, the inverter delay is 1 nanosecond.
 */
constexpr double inverter_delay = 1.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The inverter simulates a gate that performs a logical inversion of its
 * input.
 */
class inverter
{
public:

    /**
     * \brief Inverter constructor.
     *
     * \param inp       The input wire for this gate.
     * \param outp      The output wire for this gate.
     * \param delay     The optional delay in seconds.
     */
    inverter(wire* inp, wire* outp, double delay = inverter_delay);

private:
    wire* in;
    wire* out;
};

} /* namespace homesim */

#endif /*HOMESIM_INVERTER_HEADER_GUARD*/
