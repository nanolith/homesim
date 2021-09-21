/**
 * \file logic/icrom.cpp
 *
 * \brief ROM IC.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include <homesim/agenda.h>
#include <homesim/ic/rom.h>

using namespace std;

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
homesim::icrom::icrom(
    const std::vector<wire*>& addresses,
    const std::vector<std::uint8_t>& bytes,
    wire* oe, wire* ce, wire* b0, wire* b1, wire* b2, wire* b3, wire* b4,
    wire* b5, wire* b6, wire* b7, double delay)
        : rom(bytes)
        , addr(addresses)
{
    /* a zero sized rom is pointless. */
    if (addr.size() == 0)
        throw rom_mismatch_error("Zero sized rom.");

    /* calculate the correct rom size. */
    size_t expected_bytes = 1;
    for (int i = 0; i < addr.size(); ++i)
        expected_bytes *= 2;

    /* verify that we have the correct number of ROM bytes. */
    if (rom.size() != expected_bytes)
        throw rom_mismatch_error("Incorrect number of ROM bytes.");

    /* Set bus wires to high-Z. */
    b0->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b1->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b2->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b3->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b4->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b5->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b6->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b7->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    conn_type_bus = WIRE_CONNECTION_TYPE_HIGH_Z;

    /* address lines are input. */
    for (auto a : addr)
        a->add_connection(WIRE_CONNECTION_TYPE_INPUT);

    /* oe and ce are input. */
    oe->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    ce->add_connection(WIRE_CONNECTION_TYPE_INPUT);

    auto rom_update_fn = [=]() {
        /* should we output a byte to the bus? */
        if (oe->get_signal() == false && ce->get_signal() == false)
        {
            /* compute address. */
            size_t address = 0;
            for (auto i = addr.rbegin(); i != addr.rend(); ++i)
            {
                address <<= 1;
                if ((*i)->get_signal())
                    address += 1;
            }

            /* decode byte. */
            uint8_t byte = rom[address];

            /* output byte to bus. */
            b0->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x01) ? true : false);
            b1->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x02) ? true : false);
            b2->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x04) ? true : false);
            b3->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x08) ? true : false);
            b4->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x10) ? true : false);
            b5->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x20) ? true : false);
            b6->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x40) ? true : false);
            b7->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_OUTPUT,
                (byte & 0x80) ? true : false);
            conn_type_bus = WIRE_CONNECTION_TYPE_OUTPUT;
        }
        else
        {
            /* Set bus wires to high-Z. */
            b0->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b1->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b2->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b3->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b4->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b5->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b6->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            b7->change_connection_type(
                conn_type_bus, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            conn_type_bus = WIRE_CONNECTION_TYPE_HIGH_Z;
        }
    };

    auto propagate_rom_update_fn = [=]() {
        global_agenda.add(delay, rom_update_fn);
    };

    /* update the ROM state on address line change. */
    for (auto a : addr)
        a->add_action(propagate_rom_update_fn);

    /* update the ROM state on output enable change. */
    oe->add_action(propagate_rom_update_fn);

    /* update the ROM state on chip enable change. */
    ce->add_action(propagate_rom_update_fn);
}
