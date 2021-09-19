/**
 * \file logic/ic74245.cpp
 *
 * \brief IC 74245 Octal Bus Transceiver.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#include <homesim/ic/74245.h>

using namespace std;

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
homesim::ic74245::ic74245(
    wire* dir, wire* a1, wire* a2, wire* a3, wire* a4, wire* a5, wire* a6,
    wire* a7, wire* a8, wire* oe, wire* b8, wire* b7, wire* b6, wire* b5,
    wire* b4, wire* b3, wire* b2, wire* b1, double delay)
{
    a1->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a2->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a3->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a4->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a5->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a6->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a7->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    a8->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b1->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b2->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b3->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b4->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b5->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b6->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b7->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    b8->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    conn_type_a = WIRE_CONNECTION_TYPE_HIGH_Z;
    conn_type_b = WIRE_CONNECTION_TYPE_HIGH_Z;

    auto a2b = [=](wire* a, wire* b) {
        return [=]() {
            if (oe->get_signal() == false && dir->get_signal() == true)
            {
                b->set_signal(a->get_signal());
            }
        };
    };

    auto b2a = [=](wire* a, wire* b) {
        return [=]() {
            if (oe->get_signal() == false && dir->get_signal() == false)
            {
                a->set_signal(b->get_signal());
            }
        };
    };

    auto update_wires = [=]() {
        /* is OE enabled? */
        if (oe->get_signal() == false)
        {
            /* output B --> A when dir is low. */
            if (dir->get_signal() == false)
            {
                if (conn_type_b != WIRE_CONNECTION_TYPE_INPUT)
                {
                    b1->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b2->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b3->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b4->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b5->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b6->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b7->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    b8->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_INPUT, false);
                    conn_type_b = WIRE_CONNECTION_TYPE_INPUT;
                }

                if (conn_type_a != WIRE_CONNECTION_TYPE_OUTPUT)
                {
                    a1->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b1->get_signal());
                    a2->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b2->get_signal());
                    a3->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b3->get_signal());
                    a4->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b4->get_signal());
                    a5->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b5->get_signal());
                    a6->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b6->get_signal());
                    a7->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b7->get_signal());
                    a8->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        b8->get_signal());
                    conn_type_a = WIRE_CONNECTION_TYPE_OUTPUT;

                    global_agenda.add(delay, b2a(a1,b1));
                    global_agenda.add(delay, b2a(a2,b2));
                    global_agenda.add(delay, b2a(a3,b3));
                    global_agenda.add(delay, b2a(a4,b4));
                    global_agenda.add(delay, b2a(a5,b5));
                    global_agenda.add(delay, b2a(a6,b6));
                    global_agenda.add(delay, b2a(a7,b7));
                    global_agenda.add(delay, b2a(a8,b8));
                }
            }
            /* output B --> A when dir is high. */
            else
            {
                if (conn_type_b != WIRE_CONNECTION_TYPE_OUTPUT)
                {
                    b1->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a1->get_signal());
                    b2->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a2->get_signal());
                    b3->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a3->get_signal());
                    b4->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a4->get_signal());
                    b5->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a5->get_signal());
                    b6->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a6->get_signal());
                    b7->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a7->get_signal());
                    b8->change_connection_type(
                        conn_type_b, WIRE_CONNECTION_TYPE_OUTPUT,
                        a8->get_signal());
                    conn_type_b = WIRE_CONNECTION_TYPE_OUTPUT;

                    global_agenda.add(delay, a2b(a1,b1));
                    global_agenda.add(delay, a2b(a2,b2));
                    global_agenda.add(delay, a2b(a3,b3));
                    global_agenda.add(delay, a2b(a4,b4));
                    global_agenda.add(delay, a2b(a5,b5));
                    global_agenda.add(delay, a2b(a6,b6));
                    global_agenda.add(delay, a2b(a7,b7));
                    global_agenda.add(delay, a2b(a8,b8));
                }

                if (conn_type_a != WIRE_CONNECTION_TYPE_INPUT)
                {
                    a1->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a2->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a3->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a4->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a5->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a6->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a7->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    a8->change_connection_type(
                        conn_type_a, WIRE_CONNECTION_TYPE_INPUT, false);
                    conn_type_a = WIRE_CONNECTION_TYPE_INPUT;
                }
            }
        }
        /* OE is disabled. */
        else
        {
            if (conn_type_a != WIRE_CONNECTION_TYPE_HIGH_Z)
            {
                a1->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a2->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a3->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a4->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a5->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a6->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a7->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                a8->change_connection_type(
                    conn_type_a, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                conn_type_a = WIRE_CONNECTION_TYPE_HIGH_Z;
            }

            if (conn_type_b != WIRE_CONNECTION_TYPE_HIGH_Z)
            {
                b1->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b2->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b3->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b4->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b5->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b6->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b7->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                b8->change_connection_type(
                    conn_type_b, WIRE_CONNECTION_TYPE_HIGH_Z, false);
                conn_type_b = WIRE_CONNECTION_TYPE_HIGH_Z;
            }
        }
    };

    auto prop_a2b = [=](wire* a, wire* b) {
        return [=]() {
            global_agenda.add(delay, a2b(a, b));
        };
    };

    auto prop_b2a = [=](wire* a, wire* b) {
        return [=]() {
            global_agenda.add(delay, b2a(a, b));
        };
    };

    dir->add_action(update_wires);
    oe->add_action(update_wires);
    a1->add_action(prop_a2b(a1, b1));
    a2->add_action(prop_a2b(a2, b2));
    a3->add_action(prop_a2b(a3, b3));
    a4->add_action(prop_a2b(a4, b4));
    a5->add_action(prop_a2b(a5, b5));
    a6->add_action(prop_a2b(a6, b6));
    a7->add_action(prop_a2b(a7, b7));
    a8->add_action(prop_a2b(a8, b8));
    b1->add_action(prop_b2a(a1, b1));
    b2->add_action(prop_b2a(a2, b2));
    b3->add_action(prop_b2a(a3, b3));
    b4->add_action(prop_b2a(a4, b4));
    b5->add_action(prop_b2a(a5, b5));
    b6->add_action(prop_b2a(a6, b6));
    b7->add_action(prop_b2a(a7, b7));
    b8->add_action(prop_b2a(a8, b8));
}
