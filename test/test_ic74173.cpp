/**
 * \file test/test_ic74173.cpp
 *
 * \brief Unit tests for the 74LS173 IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */
#include <homesim/ic/74173.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(ic74173);

/**
 * Verify that we can clear the bits.
 */
TEST(clear_bits)
{
    wire m;
    wire n;
    wire out1q;
    wire out2q;
    wire out3q;
    wire out4q;
    wire clk;
    wire clr;
    wire in1d;
    wire in2d;
    wire in3d;
    wire in4d;
    wire g1;
    wire g2;

    ic74173 ic(
        &m, &n, &out1q, &out2q, &out3q, &out4q, &clk, &clr, &in1d, &in2d, &in3d,
        &in4d, &g1, &g2);

    /* if we bring CLR high, then all bits are cleared. */
    clr.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.get_signal() == false);
    TEST_EXPECT(out1q.is_floating() == false);
    TEST_EXPECT(out2q.get_signal() == false);
    TEST_EXPECT(out2q.is_floating() == false);
    TEST_EXPECT(out3q.get_signal() == false);
    TEST_EXPECT(out3q.is_floating() == false);
    TEST_EXPECT(out4q.get_signal() == false);
    TEST_EXPECT(out4q.is_floating() == false);
}

/**
 * Verify that we can set the bits.
 */
TEST(set_bits)
{
    wire m;
    wire n;
    wire out1q;
    wire out2q;
    wire out3q;
    wire out4q;
    wire clk;
    wire clr;
    wire in1d;
    wire in2d;
    wire in3d;
    wire in4d;
    wire g1;
    wire g2;

    ic74173 ic(
        &m, &n, &out1q, &out2q, &out3q, &out4q, &clk, &clr, &in1d, &in2d, &in3d,
        &in4d, &g1, &g2);

    m.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    n.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clk.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clr.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    g1.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    g2.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in1d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in2d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in3d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in4d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);

    /* set clr to low, clk to high, and the input registers. */
    in1d.set_signal(false);
    in2d.set_signal(false);
    in3d.set_signal(false);
    in4d.set_signal(false);
    m.set_signal(false);
    n.set_signal(false);
    g1.set_signal(false);
    g2.set_signal(false);
    clr.set_signal(false);
    clk.set_signal(false);
    propagate();
    clk.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.get_signal() == false);
    TEST_EXPECT(out1q.is_floating() == false);
    TEST_EXPECT(out2q.get_signal() == false);
    TEST_EXPECT(out2q.is_floating() == false);
    TEST_EXPECT(out3q.get_signal() == false);
    TEST_EXPECT(out3q.is_floating() == false);
    TEST_EXPECT(out4q.get_signal() == false);
    TEST_EXPECT(out4q.is_floating() == false);

    /* set clr to low, clk to high, and the input registers. */
    in1d.set_signal(true);
    in2d.set_signal(true);
    in3d.set_signal(true);
    in4d.set_signal(true);
    m.set_signal(false);
    n.set_signal(false);
    g1.set_signal(false);
    g2.set_signal(false);
    clr.set_signal(false);
    clk.set_signal(false);
    propagate();
    clk.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.get_signal() == true);
    TEST_EXPECT(out1q.is_floating() == false);
    TEST_EXPECT(out2q.get_signal() == true);
    TEST_EXPECT(out2q.is_floating() == false);
    TEST_EXPECT(out3q.get_signal() == true);
    TEST_EXPECT(out3q.is_floating() == false);
    TEST_EXPECT(out4q.get_signal() == true);
    TEST_EXPECT(out4q.is_floating() == false);
}

/**
 * when m or n is set, the outputs are floating.
 */
TEST(test_impedance)
{
    wire m;
    wire n;
    wire out1q;
    wire out2q;
    wire out3q;
    wire out4q;
    wire clk;
    wire clr;
    wire in1d;
    wire in2d;
    wire in3d;
    wire in4d;
    wire g1;
    wire g2;

    ic74173 ic(
        &m, &n, &out1q, &out2q, &out3q, &out4q, &clk, &clr, &in1d, &in2d, &in3d,
        &in4d, &g1, &g2);

    /* verify the pins are set correctly. */
    TEST_ASSERT(out1q.get_pull_downs() == 0);
    TEST_ASSERT(out1q.get_pull_ups() == 0);
    TEST_ASSERT(out1q.get_high_zs() == 0);
    TEST_ASSERT(out1q.get_inputs() == 0);
    TEST_ASSERT(out1q.get_outputs() == 1);

    /* set both m and n to low and verify that the outputs are NOT floating. */
    m.set_signal(false);
    n.set_signal(false);
    propagate();
    TEST_EXPECT(out1q.is_floating() == false);
    TEST_EXPECT(out1q.has_fault() == false);
    TEST_EXPECT(out2q.is_floating() == false);
    TEST_EXPECT(out2q.has_fault() == false);
    TEST_EXPECT(out3q.is_floating() == false);
    TEST_EXPECT(out3q.has_fault() == false);
    TEST_EXPECT(out4q.is_floating() == false);
    TEST_EXPECT(out4q.has_fault() == false);

    /* if m is high, then the outputs are floating. */
    m.set_signal(true);
    n.set_signal(false);
    propagate();
    TEST_EXPECT(out1q.is_floating() == true);
    TEST_EXPECT(out1q.has_fault() == false);
    TEST_EXPECT(out2q.is_floating() == true);
    TEST_EXPECT(out2q.has_fault() == false);
    TEST_EXPECT(out3q.is_floating() == true);
    TEST_EXPECT(out3q.has_fault() == false);
    TEST_EXPECT(out4q.is_floating() == true);
    TEST_EXPECT(out4q.has_fault() == false);

    /* if n is high, then the outputs are floating. */
    m.set_signal(false);
    n.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.is_floating() == true);
    TEST_EXPECT(out1q.has_fault() == false);
    TEST_EXPECT(out2q.is_floating() == true);
    TEST_EXPECT(out2q.has_fault() == false);
    TEST_EXPECT(out3q.is_floating() == true);
    TEST_EXPECT(out3q.has_fault() == false);
    TEST_EXPECT(out4q.is_floating() == true);
    TEST_EXPECT(out4q.has_fault() == false);

    /* if m and n are high, then the outputs are floating. */
    m.set_signal(true);
    n.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.is_floating() == true);
    TEST_EXPECT(out1q.has_fault() == false);
    TEST_EXPECT(out2q.is_floating() == true);
    TEST_EXPECT(out2q.has_fault() == false);
    TEST_EXPECT(out3q.is_floating() == true);
    TEST_EXPECT(out3q.has_fault() == false);
    TEST_EXPECT(out4q.is_floating() == true);
    TEST_EXPECT(out4q.has_fault() == false);
}

/**
 * Verify that if the bits are set, we can clear them again.
 */
TEST(set_clear)
{
    wire m;
    wire n;
    wire out1q;
    wire out2q;
    wire out3q;
    wire out4q;
    wire clk;
    wire clr;
    wire in1d;
    wire in2d;
    wire in3d;
    wire in4d;
    wire g1;
    wire g2;

    ic74173 ic(
        &m, &n, &out1q, &out2q, &out3q, &out4q, &clk, &clr, &in1d, &in2d, &in3d,
        &in4d, &g1, &g2);

    m.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    n.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clk.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clr.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    g1.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    g2.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in1d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in2d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in3d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in4d.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);

    /* set clr to low, clk to high, and the input registers. */
    in1d.set_signal(true);
    in2d.set_signal(true);
    in3d.set_signal(true);
    in4d.set_signal(true);
    m.set_signal(false);
    n.set_signal(false);
    g1.set_signal(false);
    g2.set_signal(false);
    clr.set_signal(false);
    clk.set_signal(false);
    propagate();
    clk.set_signal(true);
    propagate();
    TEST_ASSERT(out1q.get_signal() == true);
    TEST_ASSERT(out2q.get_signal() == true);
    TEST_ASSERT(out3q.get_signal() == true);
    TEST_ASSERT(out4q.get_signal() == true);

    /* clear the registers. */
    clr.set_signal(true);
    propagate();
    TEST_EXPECT(out1q.get_signal() == false);
    TEST_EXPECT(out2q.get_signal() == false);
    TEST_EXPECT(out2q.get_signal() == false);
    TEST_EXPECT(out2q.get_signal() == false);
}
