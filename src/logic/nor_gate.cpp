/**
 * \file logic/nor_gate.cpp
 *
 * \brief Nor gate constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/nor_gate.h>

using namespace homesim;
using namespace std;

/**
 * \brief Nor gate constructor.
 *
 * \param o1p       The first input for the nor gate.
 * \param o2p       The second input for the nor gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::nor_gate::nor_gate(wire* o1p, wire* o2p, wire* outp, double delay)
    : o1(o1p), o2(o2p), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( ! (o1->get_signal() || o2->get_signal()) );
        });
    };

    /* any time either of the input wires change signal, run the lambda. */
    o1->add_action(signal_proc);
    o2->add_action(signal_proc);
}
