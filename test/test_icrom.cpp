/**
 * \file test/test_icrom.cpp
 *
 * \brief Unit tests for the rom IC.
 *
 * \copyright Copyright 2021 Justin Handville.  All rights reserved.
 */

#include <homesim/agenda.h>
#include <homesim/ic/rom.h>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(icrom);

/**
 * If too few bytes are provided to the ROM constructor, then an exception is
 * thrown.
 */
TEST(rom_mismatch)
{
    wire oe;
    wire ce;
    wire a[4];
    wire bus[8];
    vector<wire*> addrs;
    vector<uint8_t> bytes;

    /* This ROM addresses sixteen bytes. */
    addrs.push_back(a + 0);
    addrs.push_back(a + 1);
    addrs.push_back(a + 2);
    addrs.push_back(a + 3);

    /* but, we are only providing one byte to the constructor. */
    bytes.push_back(1);

    try
    {
        icrom ic(
            addrs, bytes, &oe, &ce, bus + 0, bus + 1, bus + 2, bus + 3, bus + 4,
            bus + 5, bus + 6, bus + 7);
        TEST_FAILURE();
    }
    catch (rom_mismatch_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * If either CE or OE are low, then the bus is floating.
 */
TEST(floating_bus)
{
    wire oe;
    wire ce;
    wire a[4];
    wire bus[8];
    vector<wire*> addrs;
    vector<uint8_t> bytes;

    /* This ROM addresses sixteen bytes. */
    addrs.push_back(a + 0);
    addrs.push_back(a + 1);
    for (auto a : addrs)
    {
        a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
        a->set_signal(false);
    }

    /* but, we are only providing one byte to the constructor. */
    bytes.push_back(1);
    bytes.push_back(2);
    bytes.push_back(3);
    bytes.push_back(4);

    /* create the ic. */
    icrom ic(
        addrs, bytes, &oe, &ce, bus + 0, bus + 1, bus + 2, bus + 3, bus + 4,
        bus + 5, bus + 6, bus + 7);

    /* make sure the output and chip enable lines are high. */
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(true);
    ce.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ce.set_signal(true);
    propagate();

    /* the bus should be floating. */
    TEST_EXPECT(bus[0].is_floating());
    TEST_EXPECT(bus[1].is_floating());
    TEST_EXPECT(bus[2].is_floating());
    TEST_EXPECT(bus[3].is_floating());
    TEST_EXPECT(bus[4].is_floating());
    TEST_EXPECT(bus[5].is_floating());
    TEST_EXPECT(bus[6].is_floating());
    TEST_EXPECT(bus[7].is_floating());

    /* this configuration should also be HIGH_Z. */
    oe.set_signal(false);
    ce.set_signal(true);
    propagate();

    /* the bus should be floating. */
    TEST_EXPECT(bus[0].is_floating());
    TEST_EXPECT(bus[1].is_floating());
    TEST_EXPECT(bus[2].is_floating());
    TEST_EXPECT(bus[3].is_floating());
    TEST_EXPECT(bus[4].is_floating());
    TEST_EXPECT(bus[5].is_floating());
    TEST_EXPECT(bus[6].is_floating());
    TEST_EXPECT(bus[7].is_floating());

    /* this configuration should also be HIGH_Z. */
    oe.set_signal(true);
    ce.set_signal(false);
    propagate();

    /* the bus should be floating. */
    TEST_EXPECT(bus[0].is_floating());
    TEST_EXPECT(bus[1].is_floating());
    TEST_EXPECT(bus[2].is_floating());
    TEST_EXPECT(bus[3].is_floating());
    TEST_EXPECT(bus[4].is_floating());
    TEST_EXPECT(bus[5].is_floating());
    TEST_EXPECT(bus[6].is_floating());
    TEST_EXPECT(bus[7].is_floating());
}

/**
 * If both CE and OE are high, then the bus outputs the address value.
 */
TEST(output_byte)
{
    wire oe;
    wire ce;
    wire a[4];
    wire bus[8];
    vector<wire*> addrs;
    vector<uint8_t> bytes;

    /* This ROM addresses sixteen bytes. */
    addrs.push_back(a + 0);
    addrs.push_back(a + 1);
    for (auto a : addrs)
    {
        a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
        a->set_signal(false);
    }

    /* but, we are only providing one byte to the constructor. */
    bytes.push_back(1);
    bytes.push_back(2);
    bytes.push_back(3);
    bytes.push_back(4);

    /* create the ic. */
    icrom ic(
        addrs, bytes, &oe, &ce, bus + 0, bus + 1, bus + 2, bus + 3, bus + 4,
        bus + 5, bus + 6, bus + 7);

    /* make sure the output and chip enable lines are low. */
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(false);
    ce.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ce.set_signal(false);
    propagate();

    /* the bus should be outputting a valid value. */
    TEST_EXPECT(!bus[0].is_floating());
    TEST_EXPECT(bus[0].get_signal() == true);
    TEST_EXPECT(!bus[1].is_floating());
    TEST_EXPECT(bus[1].get_signal() == false);
    TEST_EXPECT(!bus[2].is_floating());
    TEST_EXPECT(bus[2].get_signal() == false);
    TEST_EXPECT(!bus[3].is_floating());
    TEST_EXPECT(bus[3].get_signal() == false);
    TEST_EXPECT(!bus[4].is_floating());
    TEST_EXPECT(bus[4].get_signal() == false);
    TEST_EXPECT(!bus[5].is_floating());
    TEST_EXPECT(bus[5].get_signal() == false);
    TEST_EXPECT(!bus[6].is_floating());
    TEST_EXPECT(bus[6].get_signal() == false);
    TEST_EXPECT(!bus[7].is_floating());
    TEST_EXPECT(bus[7].get_signal() == false);
}

/**
 * Verify that we can output every byte.
 */
TEST(output_all_bytes)
{
    wire oe;
    wire ce;
    wire a[4];
    wire bus[8];
    vector<wire*> addrs;
    vector<uint8_t> bytes;

    /* This ROM addresses sixteen bytes. */
    addrs.push_back(a + 0);
    addrs.push_back(a + 1);
    for (auto a : addrs)
    {
        a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
        a->set_signal(false);
    }

    /* but, we are only providing one byte to the constructor. */
    bytes.push_back(1);
    bytes.push_back(2);
    bytes.push_back(3);
    bytes.push_back(4);

    /* create the ic. */
    icrom ic(
        addrs, bytes, &oe, &ce, bus + 0, bus + 1, bus + 2, bus + 3, bus + 4,
        bus + 5, bus + 6, bus + 7);

    /* make sure the output and chip enable lines are low. */
    oe.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    oe.set_signal(false);
    ce.add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ce.set_signal(false);
    propagate();

    /* the bus should be outputting the first byte. */
    TEST_EXPECT(!bus[0].is_floating());
    TEST_EXPECT(bus[0].get_signal() == true);
    TEST_EXPECT(!bus[1].is_floating());
    TEST_EXPECT(bus[1].get_signal() == false);
    TEST_EXPECT(!bus[2].is_floating());
    TEST_EXPECT(bus[2].get_signal() == false);
    TEST_EXPECT(!bus[3].is_floating());
    TEST_EXPECT(bus[3].get_signal() == false);
    TEST_EXPECT(!bus[4].is_floating());
    TEST_EXPECT(bus[4].get_signal() == false);
    TEST_EXPECT(!bus[5].is_floating());
    TEST_EXPECT(bus[5].get_signal() == false);
    TEST_EXPECT(!bus[6].is_floating());
    TEST_EXPECT(bus[6].get_signal() == false);
    TEST_EXPECT(!bus[7].is_floating());
    TEST_EXPECT(bus[7].get_signal() == false);

    /* encode the second byte address. */
    addrs[0]->set_signal(true);
    addrs[1]->set_signal(false);
    propagate();

    /* the bus should be outputting the second byte. */
    TEST_EXPECT(!bus[0].is_floating());
    TEST_EXPECT(bus[0].get_signal() == false);
    TEST_EXPECT(!bus[1].is_floating());
    TEST_EXPECT(bus[1].get_signal() == true);
    TEST_EXPECT(!bus[2].is_floating());
    TEST_EXPECT(bus[2].get_signal() == false);
    TEST_EXPECT(!bus[3].is_floating());
    TEST_EXPECT(bus[3].get_signal() == false);
    TEST_EXPECT(!bus[4].is_floating());
    TEST_EXPECT(bus[4].get_signal() == false);
    TEST_EXPECT(!bus[5].is_floating());
    TEST_EXPECT(bus[5].get_signal() == false);
    TEST_EXPECT(!bus[6].is_floating());
    TEST_EXPECT(bus[6].get_signal() == false);
    TEST_EXPECT(!bus[7].is_floating());
    TEST_EXPECT(bus[7].get_signal() == false);

    /* encode the third byte address. */
    addrs[0]->set_signal(false);
    addrs[1]->set_signal(true);
    propagate();

    /* the bus should be outputting the third byte. */
    TEST_EXPECT(!bus[0].is_floating());
    TEST_EXPECT(bus[0].get_signal() == true);
    TEST_EXPECT(!bus[1].is_floating());
    TEST_EXPECT(bus[1].get_signal() == true);
    TEST_EXPECT(!bus[2].is_floating());
    TEST_EXPECT(bus[2].get_signal() == false);
    TEST_EXPECT(!bus[3].is_floating());
    TEST_EXPECT(bus[3].get_signal() == false);
    TEST_EXPECT(!bus[4].is_floating());
    TEST_EXPECT(bus[4].get_signal() == false);
    TEST_EXPECT(!bus[5].is_floating());
    TEST_EXPECT(bus[5].get_signal() == false);
    TEST_EXPECT(!bus[6].is_floating());
    TEST_EXPECT(bus[6].get_signal() == false);
    TEST_EXPECT(!bus[7].is_floating());
    TEST_EXPECT(bus[7].get_signal() == false);

    /* encode the fourth byte address. */
    addrs[0]->set_signal(true);
    addrs[1]->set_signal(true);
    propagate();

    /* the bus should be outputting the fourth byte. */
    TEST_EXPECT(!bus[0].is_floating());
    TEST_EXPECT(bus[0].get_signal() == false);
    TEST_EXPECT(!bus[1].is_floating());
    TEST_EXPECT(bus[1].get_signal() == false);
    TEST_EXPECT(!bus[2].is_floating());
    TEST_EXPECT(bus[2].get_signal() == true);
    TEST_EXPECT(!bus[3].is_floating());
    TEST_EXPECT(bus[3].get_signal() == false);
    TEST_EXPECT(!bus[4].is_floating());
    TEST_EXPECT(bus[4].get_signal() == false);
    TEST_EXPECT(!bus[5].is_floating());
    TEST_EXPECT(bus[5].get_signal() == false);
    TEST_EXPECT(!bus[6].is_floating());
    TEST_EXPECT(bus[6].get_signal() == false);
    TEST_EXPECT(!bus[7].is_floating());
    TEST_EXPECT(bus[7].get_signal() == false);
}
