/**
 * \file test/test_or_gate.cpp
 *
 * \brief Unit tests for the or gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/or_gate.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(or_gate);

/**
 * Verify the truth table for an OR gate.
 */
TEST(truth_table)
{
    wire lhs;
    wire rhs;
    wire out;
    or_gate gate(&lhs, &rhs, &out);

    propagate();

    /* false or false = false. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* false or true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true or false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true or true = true. */
    lhs.set_signal(true);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify that the correct propagation delay is set by default.
 */
TEST(propagation_time_default)
{
    /* force the global agenda into a known state. */
    global_agenda.clear();

    /* the default or gate delay is one nanosecond. */
    TEST_EXPECT(or_gate_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create an or gate. */
    wire lhs;
    wire rhs;
    wire out;
    or_gate gate(&lhs, &rhs, &out);

    /* propagate the initial signal for the or gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_EXPECT(global_agenda.current_time() == or_gate_delay);
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

    /* create an or gate. */
    wire lhs;
    wire rhs;
    wire out;
    or_gate gate(&lhs, &rhs, &out, 5.0);

    /* propagate the initial signal for the nor gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
