/**
 * \file logic/xnor_gate.cpp
 *
 * \brief Xnor gate constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/xnor_gate.h>

using namespace homesim;
using namespace std;

/**
 * \brief Xnor gate constructor.
 *
 * \param x1p       The first input for the nor gate.
 * \param x2p       The second input for the nor gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::xnor_gate::xnor_gate(wire* x1p, wire* x2p, wire* outp, double delay)
    : x1(x1p), x2(x2p), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( ! (x1->get_signal() ^ x2->get_signal()) );
        });
    };

    /* any time either of the input wires change signal, run the lambda. */
    x1->add_action(signal_proc);
    x2->add_action(signal_proc);
}
