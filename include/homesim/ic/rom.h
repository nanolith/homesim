/**
 * \file homesim/ic/rom.h
 *
 * \brief Declarations for ROM ICs.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_ROM_HEADER_GUARD
# define HOMESIM_IC_ROM_HEADER_GUARD

#include <cstdint>
#include <homesim/constants.h>
#include <homesim/wire.h>
#include <vector>

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

namespace homesim {

/**
 * \brief ROM output change delay.
 *
 * By default, this delay mimics worst-case performance for ROM.
 */
constexpr double icrom_delay = 8.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ROM mismatch error occurs when number of bytes provided to a rom
 * constructor does not match the computed number of bytes addressable with the
 * given address lines.
 */
class rom_mismatch_error : public std::runtime_error
{
public:
    rom_mismatch_error(const std::string what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief The icrom simulates a parallel ROM interface.
 */
class icrom
{
public:

    /**
     * \brief icrom constructor.
     *
     * The user provides a vector of address line wire pointers, a vector of
     * memory bytes starting at a zero index, a chip enable wire, an output
     * enable wire, and the eight bus wires.  It is an error if there are not
     * enough memory bytes to cover the number of possible addresses specified
     * by the address line wire pointers.  For instance, if four pointers are
     * provided, then there must be sixteen ROM bytes available.
     *
     * \param addresses         Vector of address wire pointers.
     * \param bytes             Vector of ROM bytes.
     * \param oe                Output Enable wire (low = output bytes; high =
     *                          bus line is high Z).
     * \param ce                Chip Enable wire (low = chip enabled; high = 
     *                          bus line is high Z).
     * \param b0                Bus line 0.
     * \param b1                Bus line 1.
     * \param b2                Bus line 2.
     * \param b3                Bus line 3.
     * \param b4                Bus line 4.
     * \param b5                Bus line 5.
     * \param b6                Bus line 6.
     * \param b7                Bus line 7.
     * \param delay             The optional delay in seconds.
     */
    icrom(
        const std::vector<wire*>& addresses,
        const std::vector<std::uint8_t>& bytes,
        wire* oe, wire* ce, wire* b0, wire* b1, wire* b2, wire* b3, wire* b4,
        wire* b5, wire* b6, wire* b7, double delay = icrom_delay);

private:
    std::vector<std::uint8_t> rom;
    std::vector<wire*> addr;
    wire_connection_type conn_type_bus;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_ROM_HEADER_GUARD*/
