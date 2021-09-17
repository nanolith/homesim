/**
 * \file test/test_ic7432.cpp
 *
 * \brief Unit tests for the 74LS32 IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */
#include <homesim/ic/7432.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(ic7432);

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

    ic7432 ic(
        &lhs, &rhs, &out, &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_out2, &dummy_lhs2, &dummy_rhs2, &dummy_out3, &dummy_lhs3,
        &dummy_rhs3);

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

    ic7432 ic(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1, &lhs, &rhs, &out,
        &dummy_out2, &dummy_lhs2, &dummy_rhs2, &dummy_out3, &dummy_lhs3,
        &dummy_rhs3);

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

    ic7432 ic(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_lhs2, &dummy_rhs2, &dummy_out2,
        &out, &lhs, &rhs, &dummy_out3, &dummy_lhs3, &dummy_rhs3);

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

    ic7432 ic(
        &dummy_lhs1, &dummy_rhs1, &dummy_out1,
        &dummy_lhs2, &dummy_rhs2, &dummy_out2,
        &dummy_out3, &dummy_lhs3, &dummy_rhs3, &out, &lhs, &rhs);

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
