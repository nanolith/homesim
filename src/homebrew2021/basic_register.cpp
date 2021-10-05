/**
 * \file basic_register.cpp
 *
 * \brief basic_register constructor.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include "basic_register.h"

using namespace homesim;
using namespace std;

/**
 * \brief Constructor for the basic register.
 */
homebrew2021::basic_register::basic_register(
    wire* clock, wire* clear,
    wire* read, wire* write,
    wire* in1, wire* in2, wire* in3, wire* in4,
    wire* in5, wire* in6, wire* in7, wire* in8,
    wire* out1, wire* out2, wire* out3, wire* out4,
    wire* out5, wire* out6, wire* out7, wire* out8)
{
    read_wire = make_shared<wire>();
    write_wire = make_shared<wire>();
    read_inv = make_shared<inverter>(read, read_wire.get());
    write_inv = make_shared<inverter>(write, write_wire.get());

    /* create the low register. */
    reg[0] = make_shared<ic74173>(
        read_wire.get(), read_wire.get(), out1, out2, out3, out4,
        clock, clear, in1, in2, in3, in4, write_wire.get(), write_wire.get());

    /* create the high register. */
    reg[1] = make_shared<ic74173>(
        read_wire.get(), read_wire.get(), out5, out6, out7, out8,
        clock, clear, in5, in6, in7, in8, write_wire.get(), write_wire.get());
}
