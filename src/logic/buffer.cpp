/**
 * \file logic/buffer.cpp
 *
 * \brief Buffer gate constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/buffer.h>

using namespace homesim;
using namespace std;

/**
 * \brief Buffer constructor.
 *
 * \param inp       The input wire for this gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::buffer::buffer(wire* inp, wire* outp, double delay)
    : in(inp), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( in->get_signal() );
        });
    };

    /* any time either of the input wires change signal, run the lambda. */
    in->add_action(signal_proc);
}
