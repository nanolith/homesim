/**
 * \file main.cpp
 *
 * \brief Main entry point for homebrew2021.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include <cassert>
#include <homesim/wire.h>
#include <iostream>

#include "basic_register.h"
#include "data_bus.h"

using namespace homebrew2021;
using namespace homesim;
using namespace std;

/* forward decls. */
static void
verify_register(
    basic_register* reg, data_bus* bus, wire* clock, wire* clear, wire* read,
    wire* write);

int main(int argc, char* argv[])
{
    /* create the data bus. */
    shared_ptr<data_bus> dbus = make_shared<data_bus>();

    /* create the clock wire. */
    shared_ptr<wire> clock = make_shared<wire>();
    clock->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    clock->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clock->set_signal(false);

    /* create the clear_a control wire. */
    shared_ptr<wire> ctrl_clr_a = make_shared<wire>();
    ctrl_clr_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_clr_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_clr_a->set_signal(false);

    /* create the read a control wire. */
    shared_ptr<wire> ctrl_read_a = make_shared<wire>();
    ctrl_read_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_read_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_read_a->set_signal(false);

    /* create the write a control wire. */
    shared_ptr<wire> ctrl_write_a = make_shared<wire>();
    ctrl_write_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_write_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_write_a->set_signal(false);

    /* create the clear_b control wire. */
    shared_ptr<wire> ctrl_clr_b = make_shared<wire>();
    ctrl_clr_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_clr_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_clr_b->set_signal(false);

    /* create the read b control wire. */
    shared_ptr<wire> ctrl_read_b = make_shared<wire>();
    ctrl_read_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_read_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_read_b->set_signal(false);

    /* create the write b control wire. */
    shared_ptr<wire> ctrl_write_b = make_shared<wire>();
    ctrl_write_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_write_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_write_b->set_signal(false);

    /* create the A register. */
    shared_ptr<basic_register> areg =
        make_shared<basic_register>(
            dbus.get(), clock.get(), ctrl_clr_a.get(), ctrl_read_a.get(),
            ctrl_write_a.get());

    /* create the B register. */
    shared_ptr<basic_register> breg =
        make_shared<basic_register>(
            dbus.get(), clock.get(), ctrl_clr_b.get(), ctrl_read_b.get(),
            ctrl_write_b.get());

    /* before starting, propagate... */
    propagate();

    /* verify the a register. */
    verify_register(
        areg.get(), dbus.get(), clock.get(), ctrl_clr_a.get(),
        ctrl_read_a.get(), ctrl_write_a.get());

    /* verify the b register. */
    verify_register(
        breg.get(), dbus.get(), clock.get(), ctrl_clr_a.get(),
        ctrl_read_a.get(), ctrl_write_a.get());

    return 0;
}

static void
verify_register(
    basic_register* reg, data_bus* bus, wire* clock, wire* clear, wire* read,
    wire* write)
{
    /* the bus should start low. */
    for (int i = 0; i < 8; ++i)
    {
        assert(bus->get_wire(i)->get_signal() == false);
    }

    /* write all ones to the register. */
    for (int i = 0; i < 8; ++i)
    {
        bus->get_wire(i)->set_signal(true);
    }

    /* turn on write. */
    write->set_signal(true);
    propagate();

    /* pulse the clock. */
    clock->set_signal(true);
    propagate();

    /* turn off clock. */
    clock->set_signal(false);
    propagate();

    /* turn off write. */
    write->set_signal(false);
    propagate();

    /* turn on read. */
    read->set_signal(true);
    propagate();

    /* pulse clock. */
    clock->set_signal(true);
    propagate();

    /* the register should be output to the bus. */
    for (int i = 0; i < 8; ++i)
    {
        assert(bus->get_wire(i)->get_signal() == true);
    }

    /* turn off clock. */
    clock->set_signal(false);
    propagate();

    /* turn off read. */
    read->set_signal(false);
    propagate();

    /* turn on clear. */
    clear->set_signal(true);
    propagate();

    /* turn off clear. */
    clear->set_signal(false);
    propagate();

    /* turn on read. */
    read->set_signal(true);
    propagate();

    /* pulse the clock. */
    clock->set_signal(true);
    propagate();

    /* the register should be output to the bus. */
    for (int i = 0; i < 8; ++i)
    {
        assert(bus->get_wire(i)->get_signal() == false);
    }

    /* turn off the clock. */
    clock->set_signal(false);
    propagate();

    /* turn off read. */
    read->set_signal(false);
    propagate();
}
