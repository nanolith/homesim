/**
 * \file test/test_buffer.cpp
 *
 * \brief Unit tests for buffer gate.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/buffer.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(buffer);

/**
 * Verify the truth table for a buffer gate.
 */
TEST(truth_table)
{
    wire in;
    wire out;
    buffer gate(&in, &out);

    propagate();

    /* false --> false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* true --> true. */
    in.set_signal(true);
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

    /* the default buffer gate delay is one nanosecond. */
    TEST_EXPECT(buffer_delay == 1.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create a buffer gate. */
    wire in;
    wire out;
    buffer gate(&in, &out);

    /* propagate the initial signal for the buffer gate. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == buffer_delay);
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

    /* create a buffer gate. */
    wire in;
    wire out;
    buffer gate(&in, &out, 5.0);

    /* propagate the initial signal for the buffer gate. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * propagation delay set in the constructor. */
    TEST_EXPECT(global_agenda.current_time() == 5.0);
}
