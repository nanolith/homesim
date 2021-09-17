/**
 * \file test/test_ic7404.cpp
 *
 * \brief Unit tests for the 74LS404 IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */
#include <homesim/ic/7404.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(ic7404);

/**
 * Verify the truth table for the first gate.
 */
TEST(gate1)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &in, &out, &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_out3, &dummy_in3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify the truth table for the second gate.
 */
TEST(gate2)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &dummy_in1, &dummy_out1, &in, &out, &dummy_in2, &dummy_out2,
        &dummy_out3, &dummy_in3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify the truth table for the third gate.
 */
TEST(gate3)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2, &in, &out,
        &dummy_out3, &dummy_in3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify the truth table for the fourth gate.
 */
TEST(gate4)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_in3, &dummy_out3, &out, &in, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify the truth table for the fifth gate.
 */
TEST(gate5)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_in3, &dummy_out3, &dummy_out4, &dummy_in4, &out, &in,
        &dummy_out5, &dummy_in5);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);
}

/**
 * Verify the truth table for the sixth gate.
 */
TEST(gate6)
{
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;

    ic7404 ic(
        &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_in3, &dummy_out3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5, &out, &in);

    /* inv false = true. */
    in.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);

    /* inv true = false. */
    in.set_signal(false);
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

    /* the default IC propagation delay is 22 nanoseconds. */
    TEST_EXPECT(ic7404_delay == 22.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create 7404 ic. */
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;
    ic7404 ic(
        &in, &out, &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_out3, &dummy_in3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5);

    /* propagate the initial signal for the ic. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == ic7404_delay);
}

/**
 * Verify that the propagation delay can be overridden in the constructor.
 */
TEST(propagation_time_override)
{
    double EXPECTED_PROPAGATION_DELAY = 44.0 * nanoseconds_to_seconds_scale;

    /* force the global agenda into a known state. */
    global_agenda.clear();

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create 7404 ic. */
    wire in;
    wire out;
    wire dummy_in1;
    wire dummy_in2;
    wire dummy_in3;
    wire dummy_in4;
    wire dummy_in5;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    wire dummy_out4;
    wire dummy_out5;
    ic7404 ic(
        &in, &out, &dummy_in1, &dummy_out1, &dummy_in2, &dummy_out2,
        &dummy_out3, &dummy_in3, &dummy_out4, &dummy_in4,
        &dummy_out5, &dummy_in5, EXPECTED_PROPAGATION_DELAY);

    /* propagate the initial signal for the ic. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == EXPECTED_PROPAGATION_DELAY);
}
