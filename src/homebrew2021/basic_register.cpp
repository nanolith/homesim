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

homebrew2021::basic_register::basic_register(
    data_bus* bus, wire* clock, wire* clear, wire* read, wire* write)
{
    low_wire = make_shared<wire>();
    low_wire->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    low_wire->set_signal(false);

    read_wire = make_shared<wire>();
    write_wire = make_shared<wire>();
    read_inv = make_shared<inverter>(read, read_wire.get());
    write_inv = make_shared<inverter>(write, write_wire.get());

    reg[0] = make_shared<ic74173>(
        read_wire.get(), low_wire.get(), bus->get_wire(0), bus->get_wire(1),
        bus->get_wire(2), bus->get_wire(3), clock, clear, bus->get_wire(0),
        bus->get_wire(1), bus->get_wire(2), bus->get_wire(3), write_wire.get(),
        write_wire.get());
    reg[1] = make_shared<ic74173>(
        read_wire.get(), low_wire.get(), bus->get_wire(4), bus->get_wire(5),
        bus->get_wire(6), bus->get_wire(7), clock, clear, bus->get_wire(4),
        bus->get_wire(5), bus->get_wire(6), bus->get_wire(7), write_wire.get(),
        write_wire.get());
}
