/**
 * \file test/test_inverter.cpp
 *
 * \brief Unit tests for inverter gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/inverter.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(inverter);

/**
 * Verify the truth table for an inverter gate.
 */
TEST(truth_table)
{
    wire in;
    wire out;
    inverter gate(&in, &out);

    propagate();

    /* !false = true. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* !true = false. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);
}

/**
 * Verify that the correct propagation delay is set by default.
 */
TEST(propagation_time_default)
{
    /* force the global agenda into a known state. */
    global_agenda.clear();

    /* the default inverter gate delay is one nanosecond. */
    TEST_EXPECT(inverter_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create an inverter gate. */
    wire in;
    wire out;
    inverter gate(&in, &out);

    /* propagate the initial signal for the inverter gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == inverter_delay);
}

/**
 * Verify that the propagation delay can be overridden in the constructor.
 */
TEST(propagation_time_override)
{
    /* force the global agenda into a known state. */
    global_agenda.clear();

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create an inverter gate. */
    wire in;
    wire out;
    inverter gate(&in, &out, 5.0);

    /* propagate the initial signal for the inverter gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
