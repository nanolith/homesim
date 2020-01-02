/**
 * \file logic/inverter.cpp
 *
 * Inverter constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/inverter.h>

using namespace homesim;
using namespace std;

/**
 * \brief Inverter constructor.
 *
 * \param inp       The input wire for this gate.
 * \param outp      The output wire for this gate.
 * \param delay     The optional delay in seconds.
 */
homesim::inverter::inverter(wire* inp, wire* outp, double delay)
    : in(inp), out(outp)
{
    /* Lambda expression for changing the output wire signal. */
    auto signal_proc = [=]() {
        /* on input change, schedule an output change after our delay. */
        global_agenda.add(delay, [=]() {
            out->set_signal( !in->get_signal() );
        });
    };

    /* Any time the input wire changes signal, run the lambda. */
    in->add_action(signal_proc);
}
