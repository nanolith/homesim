/**
 * \file test/test_xnor_gate.cpp
 *
 * \brief Unit tests for the xnor gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/xnor_gate.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(xnor_gate);

/**
 * Verify the truth table for an XNOR gate.
 */
TEST(truth_table)
{
    wire lhs;
    wire rhs;
    wire out;
    xnor_gate gate(&lhs, &rhs, &out);

    propagate();

    /* false xnor false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false xnor true = false. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* true xnor false = false. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* true xnor true = true. */
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

    /* the default xnor gate delay is one nanosecond. */
    TEST_EXPECT(xnor_gate_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create an xnor gate. */
    wire lhs;
    wire rhs;
    wire out;
    xnor_gate gate(&lhs, &rhs, &out);

    /* propagate the initial signal for the xnor gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == xnor_gate_delay);
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

    /* create an xnor gate. */
    wire lhs;
    wire rhs;
    wire out;
    xnor_gate gate(&lhs, &rhs, &out, 5.0);

    /* propagate the initial signal for the xnor gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
