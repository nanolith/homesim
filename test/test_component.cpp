/**
 * \file test/test_component.cpp
 *
 * \brief Unit tests for the component.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <minunit/minunit.h>
#include "test_component.h"

using namespace homesim;
using namespace std;

TEST_SUITE(component);

/* it is possible to create a component and interrogate it. */
TEST(construction)
{
    test_component c;

    TEST_ASSERT(6 == c.pins());
    TEST_EXPECT(string("1a") == c.pin_name(0));
    TEST_EXPECT(string("1b") == c.pin_name(1));
    TEST_EXPECT(string("1y") == c.pin_name(2));
    TEST_EXPECT(string("2a") == c.pin_name(3));
    TEST_EXPECT(string("2b") == c.pin_name(4));
    TEST_EXPECT(string("2y") == c.pin_name(5));
}

/* Setting an invalid pin name to a wire value throws an exception. */
TEST(invalid_pin_error)
{
    test_component c;
    wire w;

    try
    {
        c.set_pin("foo", &w);

        /* the set_pin call above should have thrown. */
        TEST_FAILURE();
    }
    catch (invalid_pin_error& e)
    {
        TEST_SUCCESS();
    }
}

/* Setting a valid pin should succeed. */
TEST(set_pin)
{
    test_component c;
    wire w;

    c.set_pin("1a", &w);
}

/* Attempting to bind the pin twice causes an error. */
TEST(pin_binding_error)
{
    test_component c;
    wire w;

    c.set_pin("1a", &w);

    try
    {
        c.set_pin("1a", &w);

        /* the second set should have thrown a binding error. */
        TEST_FAILURE();
    }
    catch (pin_binding_error& e)
    {
        TEST_SUCCESS();
    }
}

/* Attempting to build the test component without setting the wires should cause
 * a missing_wire_error. */
TEST(missing_wire_error)
{
    test_component c;

    try
    {
        c.build();

        /* a missing_wire_error should have been thrown. */
        TEST_FAILURE();
    }
    catch (missing_wire_error& e)
    {
        TEST_SUCCESS();
    }
}

/* it should be possible to "build" the test component after setting each of the
 * wires. */
TEST(build)
{
    test_component c;
    wire w1a, w1b, w1y, w2a, w2b, w2y;

    c.set_pin("1a", &w1a);
    c.set_pin("1b", &w1b);
    c.set_pin("1y", &w1y);
    c.set_pin("2a", &w2a);
    c.set_pin("2b", &w2b);
    c.set_pin("2y", &w2y);
    c.build();
}
