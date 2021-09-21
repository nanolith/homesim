/**
 * \file logic/ic74173.cpp
 *
 * \brief IC 74173 (Quad D-type Register) constructor.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#include <homesim/ic/74173.h>

using namespace std;

/**
 * \brief ic74173 constructor, with parameters in roughly DIP package order.
 *
 * \param m             Gate control input M.
 * \param n             Gate control input N.
 * \param out1q         Output 1Q.
 * \param out2q         Output 2Q.
 * \param out3q         Output 3Q.
 * \param out4q         Output 4Q.
 * \param clk           Clock input.
 * \param clr           Clear input.
 * \param in1d          Input 1D.
 * \param in2d          Input 2D.
 * \param in3d          Input 3D.
 * \param in4d          Input 4D.
 * \param g1            Input data-enable 1.
 * \param g2            Input data-enable 2.
 * \param delay         The optional delay in seconds.
 */
homesim::ic74173::ic74173(
    wire* m, wire* n, wire* out1q, wire* out2q, wire* out3q,
    wire* out4q, wire* clk, wire* clr, wire* in1d, wire* in2d, wire* in3d,
    wire* in4d, wire* g1, wire* g2, double delay)
{
    m->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    n->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    conn_type = WIRE_CONNECTION_TYPE_OUTPUT;
    out1q->add_connection(conn_type);
    out2q->add_connection(conn_type);
    out3q->add_connection(conn_type);
    out4q->add_connection(conn_type);
    in1d->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    in2d->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    in3d->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    in4d->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    clk->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    clr->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    g1->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    g2->add_connection(WIRE_CONNECTION_TYPE_INPUT);

    /* clear the registers to start. */
    reg[0] = false;
    reg[1] = false;
    reg[2] = false;
    reg[3] = false;

    /* Lambda expression for outputting the registers. */
    auto output_registers = [=]() {

        if (m->get_signal() == true || n->get_signal() == true)
        {
            out1q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            out2q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            out3q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            out4q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_HIGH_Z, false);
            conn_type = WIRE_CONNECTION_TYPE_HIGH_Z;
        }
        else
        {
            out1q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_OUTPUT, reg[0]);
            out2q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_OUTPUT, reg[1]);
            out3q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_OUTPUT, reg[2]);
            out4q->change_connection_type(
                conn_type, WIRE_CONNECTION_TYPE_OUTPUT, reg[3]);
            conn_type = WIRE_CONNECTION_TYPE_OUTPUT;
        }
    };

    auto propagate_output_registers = [=]() {
        global_agenda.add(delay, output_registers);
    };

    /* Lambda expression for clearing the registers. */
    auto clear_signal_proc = [=]() {

        /* when clear is set... */
        if (clr->get_signal() == true)
        {
            /* propagate reset of the registers. */
            global_agenda.add(delay, [=]() {
                reg[0] = false;
                reg[1] = false;
                reg[2] = false;
                reg[3] = false;
                output_registers();
            });
        }
    };

    /* Lambda expression for handling clock states. */
    auto clock_signal_proc = [=]() {

        /* clear must be low for the following actions to propagate. */
        if (clr->get_signal() == true)
        {
            return;
        }

        /* if the clock is low, output the registers. */
        if (clk->get_signal() == false)
        {
            global_agenda.add(delay, output_registers);
        }
        /* if either data enable pin is set, output the registers. */
        else if (g1->get_signal() == true || g2->get_signal() == true)
        {
            global_agenda.add(delay, output_registers);
        }

        if (
             clk->get_signal() == true
          && g1->get_signal() == false
          && g2->get_signal() == false)
        {
            /* in any other state, assign the register to the data input. */
            global_agenda.add(delay, [=]() {
                reg[0] = in1d->get_signal();
                reg[1] = in2d->get_signal();
                reg[2] = in3d->get_signal();
                reg[3] = in4d->get_signal();

                output_registers();
            });
        }
    };

    clr->add_action(clear_signal_proc);
    clk->add_action(clock_signal_proc);
    m->add_action(propagate_output_registers);
    n->add_action(propagate_output_registers);
}
