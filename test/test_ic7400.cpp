/**
 * \file test/test_ic7400.cpp
 *
 * \brief Unit tests for the 74LS00 IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */
#include <homesim/ic/7400.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(ic7400);

/**
 * Verify the truth table for the first gate.
 */
TEST(gate1)
{
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;

    ic7400(
        &lhs, &rhs, &out, &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_out2, &dummy_rhs2, &dummy_lhs2, &dummy_out3, &dummy_rhs3,
        &dummy_lhs3);

    /* false nand false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false nand true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand true = false. */
    lhs.set_signal(true);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);
}

/**
 * Verify the truth table for the second gate.
 */
TEST(gate2)
{
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;

    ic7400(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1, &lhs, &rhs, &out,
        &dummy_out2, &dummy_rhs2, &dummy_lhs2, &dummy_out3, &dummy_rhs3,
        &dummy_lhs3);

    /* false nand false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false nand true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand true = false. */
    lhs.set_signal(true);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);
}

/**
 * Verify the truth table for the third gate.
 */
TEST(gate3)
{
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;

    ic7400(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_lhs2, &dummy_rhs2, &dummy_out2,
        &out, &rhs, &lhs, &dummy_out3, &dummy_rhs3, &dummy_lhs3);

    /* false nand false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false nand true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand true = false. */
    lhs.set_signal(true);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == false);
}

/**
 * Verify the truth table for the fourth gate.
 */
TEST(gate4)
{
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;

    ic7400(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_lhs2, &dummy_rhs2, &dummy_out2,
        &dummy_out3, &dummy_rhs3, &dummy_lhs3, &out, &rhs, &lhs);

    /* false nand false = true. */
    lhs.set_signal(false);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* false nand true = true. */
    lhs.set_signal(false);
    rhs.set_signal(true);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand false = true. */
    lhs.set_signal(true);
    rhs.set_signal(false);
    propagate();
    TEST_EXPECT(out.get_signal() == true);

    /* true nand true = false. */
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

    /* the default IC propagation delay is 22 nanoseconds. */
    TEST_EXPECT(ic7400_delay == 22.0 * nanoseconds_to_seconds_scale);

    /* the global agenda starts at t = 0. */
    TEST_ASSERT(global_agenda.current_time() == 0.0);

    /* create 7400 ic. */
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    ic7400(
        &lhs, &rhs, &out, &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_out2, &dummy_rhs2, &dummy_lhs2, &dummy_out3, &dummy_rhs3,
        &dummy_lhs3);

    /* propagate the initial signal for the ic. */
    propagate();

    /* verify that current time according to the global agenda is our
     * propagation delay. */
    TEST_ASSERT(global_agenda.current_time() == ic7400_delay);
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

    /* create 7400 ic. */
    wire lhs;
    wire rhs;
    wire out;
    wire dummy_lhs1;
    wire dummy_lhs2;
    wire dummy_lhs3;
    wire dummy_rhs1;
    wire dummy_rhs2;
    wire dummy_rhs3;
    wire dummy_out1;
    wire dummy_out2;
    wire dummy_out3;
    ic7400(
        &lhs, &rhs, &out, &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_out2, &dummy_rhs2, &dummy_lhs2, &dummy_out3, &dummy_rhs3,
        &dummy_lhs3, EXPECTED_PROPAGATION_DELAY);

    /* propagate the initial signal for the ic. */
    propagate();

    /* verify that current time according to the global agenda is equal to the
     * value set in the constructor. */
    TEST_ASSERT(global_agenda.current_time() == EXPECTED_PROPAGATION_DELAY);
}
