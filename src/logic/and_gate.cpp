/**
 * \file logic/and_gate.cpp
 *
 * \brief And gate constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/and_gate.h>

using namespace homesim;
using namespace std;

/**
 * \brief And gate constructor.
 *
 * \param a1p       The first input for the and gate.
 * \param a2p       The second input for the and gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::and_gate::and_gate(wire* a1p, wire* a2p, wire* outp, double delay)
    : a1(a1p), a2(a2p), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( a1->get_signal() && a2->get_signal() );
        });
    };

    /* any time either of the input wires change signal, run the lambda. */
    a1->add_action(signal_proc);
    a2->add_action(signal_proc);
}
