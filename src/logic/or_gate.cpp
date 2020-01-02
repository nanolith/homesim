/**
 * \file logic/or_gate.cpp
 *
 * \brief Or gate constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/or_gate.h>

using namespace homesim;
using namespace std;

/**
 * \brief Or gate constructor.
 *
 * \param o1p       The first input for the or gate.
 * \param o2p       The second input for the or gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::or_gate::or_gate(wire* o1p, wire* o2p, wire* outp, double delay)
    : o1(o1p), o2(o2p), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( o1->get_signal() || o2->get_signal() );
        });
    };

    /* any time either of the input wires change signal, run the lambda. */
    o1->add_action(signal_proc);
    o2->add_action(signal_proc);
}
