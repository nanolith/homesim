/**
 * \file homesim/ic/74245.h
 *
 * \brief Declarations for a 74245-like Octal Bus Transceiver.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_74245_HEADER_GUARD
# define HOMESIM_IC_74245_HEADER_GUARD

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
 * \brief 74245 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS245.
 */
constexpr double ic74245_delay = 8.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic74245 simulates a 74X245 Octal Bus Transceiver.
 */
class ic74245
{
public:

    /**
     * \brief ic74245 constructor, with parameters in roughly DIP package order.
     *
     * \param dir           If high, B --> A; if low, A --> B.
     * \param a1            Channel 1, A side.
     * \param a2            Channel 2, A side.
     * \param a3            Channel 3, A side.
     * \param a4            Channel 4, A side.
     * \param a5            Channel 5, A side.
     * \param a6            Channel 6, A side.
     * \param a7            Channel 7, A side.
     * \param a8            Channel 8, A side.
     * \param oe            Active Low Output Enable (low = all channels active;
     *                      high = all channels disabled / high-Z).
     * \param b8            Channel 8, B side.
     * \param b7            Channel 7, B side.
     * \param b6            Channel 6, B side.
     * \param b5            Channel 5, B side.
     * \param b4            Channel 4, B side.
     * \param b3            Channel 3, B side.
     * \param b2            Channel 2, B side.
     * \param b1            Channel 1, B side.
     * \param delay         The optional delay in seconds.
     */
    ic74245(
        wire* dir, wire* a1, wire* a2, wire* a3, wire* a4, wire* a5, wire* a6,
        wire* a7, wire* a8, wire* oe, wire* b8, wire* b7, wire* b6, wire* b5,
        wire* b4, wire* b3, wire* b2, wire* b1, double delay = ic74245_delay);

private:
    wire_connection_type conn_type_a;
    wire_connection_type conn_type_b;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_74245_HEADER_GUARD*/
