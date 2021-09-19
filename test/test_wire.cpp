/**
 * \file test/test_wire.cpp
 *
 * \brief Unit tests for the wire simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/wire.h>
#include <memory>
#include <minunit/minunit.h>

using namespace homesim;
using namespace std;

TEST_SUITE(wire);

/**
 * By default, wire does not use the float / fault interface.
 */
TEST(fault_defaults)
{
    auto w = make_shared<wire>();

    /* By default, a wire has no faults. */
    TEST_ASSERT( !w->has_fault() );
    /* A wire with no connections is, by definition, floating. */
    TEST_ASSERT( w->is_floating() );
}

/**
 * Adding a connection enables this interface. An input with no outputs or
 * tie-down circuitry causes a floating wire.
 */
TEST(input_floating)
{
    auto w = make_shared<wire>();

    /* if we add an input, then the wire is floating because there is no
     * output, pull-up, or pull-down */
    w->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );
}

/**
 * Adding an output to such a wire ensures that it isn't floating.
 */
TEST(input_output_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add an input, then the input is floating because there is no
     * output, pull-up, or pull-down */
    w->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding an output eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Adding a pull-down to such a wire ensures that it isn't floating.
 */
TEST(input_pull_down_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add an input, then the input is floating because there is no
     * output, pull-up, or pull-down */
    w->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding a pull-down eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Adding a pull-up to such a wire ensures that it isn't floating.
 */
TEST(input_pull_up_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add an input, then the input is floating because there is no
     * output, pull-up, or pull-down */
    w->add_connection(WIRE_CONNECTION_TYPE_INPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding a pull-up eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Two outputs cause a wiring fault.
 */
TEST(output_output_fault)
{
    auto w = make_shared<wire>();

    /* if there is an output... */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add another output, this is causes a fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * An output can coexist with a pull-down.
 */
TEST(output_pull_down)
{
    auto w = make_shared<wire>();

    /* if there is an output... */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-down, there is no fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * An output can coexist with a pull-up.
 */
TEST(output_pull_up)
{
    auto w = make_shared<wire>();

    /* if there is an output... */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-down, there is no fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Multiple pull-downs causes a wiring fault.
 */
TEST(pull_down_pull_down_fault)
{
    auto w = make_shared<wire>();

    /* if there is a pull-down... */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-down, there is a fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * A pull-down and a pull-up causes a wiring fault.
 */
TEST(pull_down_pull_up_fault)
{
    auto w = make_shared<wire>();

    /* if there is a pull-down... */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-up, there is a fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * A pull-up and a pull-down causes a wiring fault.
 */
TEST(pull_up_pull_down_fault)
{
    auto w = make_shared<wire>();

    /* if there is a pull-up... */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-down, there is a fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Multiple pull-ups cause a wiring fault.
 */
TEST(pull_up_pull_up_fault)
{
    auto w = make_shared<wire>();

    /* if there is a pull-up... */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );

    /* and we add a pull-up, there is a fault. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Adding a high-Z connection causes the wire to float.
 */
TEST(high_z_floating)
{
    auto w = make_shared<wire>();

    /* if we add a high-z connection, then the wire is floating because there is
     * no output, pull-up, or pull-down. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );
}

/**
 * An output ensures that a high-z connection won't cause a wire to float.
 */
TEST(high_z_output_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add a high-z connection, then the wire is floating because there is
     * no output, pull-up, or pull-down. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding an output eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * A pull-down ensures that a high-z connection won't cause a wire to float.
 */
TEST(high_z_pull_down_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add a high-z connection, then the wire is floating because there is
     * no output, pull-up, or pull-down. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding a pull-down eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * A pull-up ensures that a high-z connection won't cause a wire to float.
 */
TEST(high_z_pull_up_no_floating)
{
    auto w = make_shared<wire>();

    /* if we add a high-z connection, then the wire is floating because there is
     * no output, pull-up, or pull-down. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( w->is_floating() );

    /* adding a pull-up eliminates floating. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_UP);
    TEST_EXPECT( !w->has_fault() );
    TEST_EXPECT( !w->is_floating() );
}

/**
 * Verify that we can register a callback to occur on any state change.
 */
TEST(state_change_callback)
{
    auto w = make_shared<wire>();
    int calls = 0;

    /* if we add state change callback, then it is called each time
     * add_connection or change_connection finishes. */
    w->add_state_change_action( [=, &calls]() {
        ++calls;
    });

    /* calls should have been incremented once when the action was added. */
    TEST_EXPECT(1 == calls);

    /* add an input. */
    w->add_connection(WIRE_CONNECTION_TYPE_INPUT);

    /* calls should have been incremented when this connection was added. */
    TEST_EXPECT(2 == calls);

    /* change the input to an output */
    w->change_connection_type(
        WIRE_CONNECTION_TYPE_INPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);

    /* calls should have been incremented again. */
    TEST_EXPECT(3 == calls);
}

/**
 * Verify that this callback can capture an output / output in a wiring with
 * dynamic connection type changes.
 */
TEST(capture_output_fault)
{
    auto w = make_shared<wire>();
    int faults = 0;

    /* add a callback to detect faults. */
    w->add_state_change_action( [=, &faults]() {
        if (w->has_fault())
            ++faults;
    });

    /* add a pull-down. */
    w->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    /* there are no faults. */
    TEST_EXPECT(0 == faults);

    /* add a high-z. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    /* there are no faults. */
    TEST_EXPECT(0 == faults);

    /* add another high-z. */
    w->add_connection(WIRE_CONNECTION_TYPE_HIGH_Z);
    /* there are no faults. */
    TEST_EXPECT(0 == faults);

    /* set a high-z to output. */
    w->change_connection_type(
        WIRE_CONNECTION_TYPE_HIGH_Z, WIRE_CONNECTION_TYPE_OUTPUT, true);
    /* there are no faults. */
    TEST_EXPECT(0 == faults);

    /* set a high-z to output. */
    w->change_connection_type(
        WIRE_CONNECTION_TYPE_HIGH_Z, WIRE_CONNECTION_TYPE_OUTPUT, false);
    /* this causes a fault. */
    TEST_EXPECT(1 == faults);

    /* rectify the problem. */
    w->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_HIGH_Z, false);
    /* no more faults occur. */
    TEST_EXPECT(1 == faults);
}
