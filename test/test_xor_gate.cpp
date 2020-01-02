/**
 * \file test/test_xor_gate.cpp
 *
 * \brief Unit tests for the xor gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/xor_gate.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(xor_gate);

/**
 * Verify the truth table for an XOR gate.
 */
TEST(truth_table)
{
    wire lhs;
    wire rhs;
    wire out;
    xor_gate gate(&lhs, &rhs, &out);

    propagate();

    /* false xor false = false. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* false xor true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true xor false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true xor true = false. */
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

    /* the default xor gate delay is one nanosecond. */
    TEST_EXPECT(xor_gate_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create an xor gate. */
    wire lhs;
    wire rhs;
    wire out;
    xor_gate gate(&lhs, &rhs, &out);

    /* propagate the initial signal for the xor gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == xor_gate_delay);
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

    /* create an xor gate. */
    wire lhs;
    wire rhs;
    wire out;
    xor_gate gate(&lhs, &rhs, &out, 5.0);

    /* propagate the initial signal for the xor gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
