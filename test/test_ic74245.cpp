/**
 * \file test/test_ic74245.cpp
 *
 * \brief Unit tests for the 74LS245 IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */
#include <homesim/ic/74245.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(ic74245);

/**
 * Verify that with direction low and output enable high, all wires are
 * floating.
 */
TEST(dir_low_oe_high_floating)
{
    wire dir;
    wire oe;
    wire a[8];
    wire b[8];

    /* All wires should be floating. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(a[i].is_floating() == true);
        TEST_EXPECT(b[i].is_floating() == true);
    }

    ic74245 ic(
        &dir, a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, a + 6, a + 7,
        &oe, b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b + 0);

    /* dir -> low; oe -> high. */
    dir.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    dir.set_signal(false);
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(true);
    propagate();

    /* All wires should be floating. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(a[i].is_floating() == true);
        TEST_EXPECT(b[i].is_floating() == true);
    }
}

/**
 * Verify that with direction high and output enable high, all wires are
 * floating.
 */
TEST(dir_high_oe_high_floating)
{
    wire dir;
    wire oe;
    wire a[8];
    wire b[8];

    /* All wires should be floating. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(a[i].is_floating() == true);
        TEST_EXPECT(b[i].is_floating() == true);
    }

    ic74245 ic(
        &dir, a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, a + 6, a + 7,
        &oe, b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b + 0);

    /* dir -> high; oe -> high. */
    dir.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    dir.set_signal(true);
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(true);
    propagate();

    /* All wires should be floating. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(a[i].is_floating() == true);
        TEST_EXPECT(b[i].is_floating() == true);
    }
}

/**
 * Verify that with direction low and output enable low, B inputs are sent to A
 * outputs.
 */
TEST(dir_low_oe_low_floating)
{
    wire dir;
    wire oe;
    wire a[8];
    wire b[8];

    /* Set B wires to output low and A wires to input / floating. */
    for (int i = 0; i < 8; ++i)
    {
        b[i].add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
        b[i].set_signal(false);
        a[i].add_connection(WIRE_CONNECTION_TYPE_INPUT);
        TEST_EXPECT(a[i].is_floating() == true);
        TEST_EXPECT(b[i].is_floating() == false);
        TEST_EXPECT(b[i].get_signal() == false);
    }

    ic74245 ic(
        &dir, a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, a + 6, a + 7,
        &oe, b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b + 0);

    /* dir -> low; oe -> low. */
    dir.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    dir.set_signal(false);
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(false);
    propagate();

    /* A wires should be low. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(a[i].is_floating() == false);
        TEST_EXPECT(a[i].get_signal() == false);
    }

    /* test channel by channel. */
    for (int i = 0; i < 8; ++i)
    {
        /* A wire should be low. */
        TEST_EXPECT(a[i].is_floating() == false);
        TEST_EXPECT(a[i].get_signal() == false);

        /* Set B wire to high. */
        b[i].set_signal(true);

        /* propagate the change. */
        propagate();

        /* A wire should be high. */
        TEST_EXPECT(a[i].is_floating() == false);
        TEST_EXPECT(a[i].get_signal() == true);
    }
}

/**
 * Verify that with direction high and output enable low, A inputs are sent to B
 * outputs.
 */
TEST(dir_high_oe_low_floating)
{
    wire dir;
    wire oe;
    wire a[8];
    wire b[8];

    /* Set A wires to output low and B wires to input / floating. */
    for (int i = 0; i < 8; ++i)
    {
        a[i].add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
        a[i].set_signal(false);
        b[i].add_connection(WIRE_CONNECTION_TYPE_INPUT);
        TEST_EXPECT(b[i].is_floating() == true);
        TEST_EXPECT(a[i].is_floating() == false);
        TEST_EXPECT(a[i].get_signal() == false);
    }

    ic74245 ic(
        &dir, a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, a + 6, a + 7,
        &oe, b + 7, b + 6, b + 5, b + 4, b + 3, b + 2, b + 1, b + 0);

    /* dir -> high; oe -> low. */
    dir.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    dir.set_signal(true);
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(false);
    propagate();

    /* B wires should be low. */
    for (int i = 0; i < 8; ++i)
    {
        TEST_EXPECT(b[i].is_floating() == false);
        TEST_EXPECT(b[i].get_signal() == false);
    }

    /* test channel by channel. */
    for (int i = 0; i < 8; ++i)
    {
        /* B wire should be low. */
        TEST_EXPECT(b[i].is_floating() == false);
        TEST_EXPECT(b[i].get_signal() == false);

        /* Set A wire to high. */
        a[i].set_signal(true);

        /* propagate the change. */
        propagate();

        /* B wire should be high. */
        TEST_EXPECT(b[i].is_floating() == false);
        TEST_EXPECT(b[i].get_signal() == true);
    }
}
