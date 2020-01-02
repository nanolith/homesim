/**
 * \file test/test_nor_gate.cpp
 *
 * \brief Unit tests for nor gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/nor_gate.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(nor_gate);

/**
 * Verify the truth table for a NOR gate.
 */
TEST(truth_table)
{
    wire lhs;
    wire rhs;
    wire out;
    nor_gate gate(&lhs, &rhs, &out);

    propagate();

    /* false nor false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false nor true = false. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* true nor false = false. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* true nor true = false. */
    lhs.set_signal(true);
    rhs.set_signal(true);
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

    /* the default nor gate delay is one nanosecond. */
    TEST_EXPECT(nor_gate_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create a nor gate. */
    wire lhs;
    wire rhs;
    wire out;
    nor_gate gate(&lhs, &rhs, &out);

    /* propagate the initial signal for the nor gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == nor_gate_delay);
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

    /* create a nor gate. */
    wire lhs;
    wire rhs;
    wire out;
    nor_gate gate(&lhs, &rhs, &out, 5.0);

    /* propagate the initial signal for the nor gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
