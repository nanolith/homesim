/**
 * \file bus_register.cpp
 *
 * \brief bus_register constructor.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include "bus_register.h"

using namespace homesim;
using namespace std;

homebrew2021::bus_register::bus_register(
    data_bus* bus, wire* clock, wire* clear, wire* read, wire* write)
{
    /* create data wires for the register. */
    for (int i = 0; i < 8; ++i)
    {
        auto db = make_shared<wire>();
        data_wires.push_back(db);
    }

    low_wire = make_shared<wire>();
    low_wire->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    low_wire->set_signal(false);

    high_wire = make_shared<wire>();
    high_wire->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    high_wire->set_signal(true);

    read_wire = make_shared<wire>();
    write_wire = make_shared<wire>();
    read_inv = make_shared<inverter>(read, read_wire.get());
    write_inv = make_shared<inverter>(write, write_wire.get());

    /* create a bus transceiver. */
    transceiver =
        make_shared<ic74245>(
            high_wire.get(), data_wires[0].get(), data_wires[1].get(), 
            data_wires[2].get(), data_wires[3].get(), data_wires[4].get(), 
            data_wires[5].get(), data_wires[6].get(), data_wires[7].get(), 
            read_wire.get(), bus->get_wire(0), bus->get_wire(1),
            bus->get_wire(2), bus->get_wire(3), bus->get_wire(4),
            bus->get_wire(5), bus->get_wire(6), bus->get_wire(7));

    /* create the low register. */
    reg[0] = make_shared<ic74173>(
        low_wire.get(), low_wire.get(), data_wires[0].get(),
        data_wires[1].get(), data_wires[2].get(), data_wires[3].get(),
        clock, clear, bus->get_wire(0), bus->get_wire(1), bus->get_wire(2),
        bus->get_wire(3), write_wire.get(), write_wire.get());

    /* create the high register. */
    reg[1] = make_shared<ic74173>(
        low_wire.get(), low_wire.get(), data_wires[4].get(),
        data_wires[5].get(), data_wires[6].get(), data_wires[7].get(),
        clock, clear, bus->get_wire(4), bus->get_wire(5), bus->get_wire(6),
        bus->get_wire(7), write_wire.get(), write_wire.get());
}
