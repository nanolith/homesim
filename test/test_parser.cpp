/**
 * \file test/test_parser.cpp
 *
 * \brief Unit tests for the parser.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>
#include <memory>
#include <minunit/minunit.h>
#include <sstream>

using namespace homesim;
using namespace std;

TEST_SUITE(parser);

/**
 * EOF throws an exception.
 */
TEST(eof)
{
    stringstream in("");
    parser p(in);

    try {
        auto res = p.parse();

        /* we expected an exception to be thrown, and it wasn't. */
        TEST_FAILURE();
    }
    catch (parser_error& e)
    {
    }
}

/**
 * An empty module can be parsed.
 */
TEST(empty_module)
{
    stringstream in("module foo { }");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());
}

/**
 * A module with a simple component can be parsed.
 */
TEST(simple_component)
{
    stringstream in("module foo { component bar { } }");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_EXPECT(0 == barcomp->config_map.size());
}

/**
 * A module with a component with a type can be parsed.
 */
TEST(simple_component_with_type)
{
    stringstream in("module foo { component bar { type x } }");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(!!barcomp->type);
    TEST_EXPECT(string("x") == *barcomp->type);
    TEST_EXPECT(0 == barcomp->config_map.size());
}

/**
 * A module with a simple wire can be parsed.
 */
TEST(simple_wire)
{
    stringstream in("module foo { wire bar { } }");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bar"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bar") == barwire->name);
    TEST_EXPECT(0 == barwire->connection_list.size());
    TEST_EXPECT(!barwire->pullup_pulldown);
    TEST_EXPECT(false == barwire->exported);
    TEST_EXPECT(false == barwire->external_source);
}

/**
 * A simple wire can be exported.
 */
TEST(export_wire)
{
    stringstream in("module foo { export wire bar { } }");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bar"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bar") == barwire->name);
    TEST_EXPECT(0 == barwire->connection_list.size());
    TEST_EXPECT(!barwire->pullup_pulldown);
    TEST_EXPECT(true == barwire->exported);
    TEST_EXPECT(false == barwire->external_source);
}

/**
 * A wire can have an external signal source.
 */
TEST(wire_with_external_signal_source)
{
    stringstream in(
        R"TEST(
            module foo {
                export wire bar {
                    signal source external
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bar"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bar") == barwire->name);
    TEST_EXPECT(0 == barwire->connection_list.size());
    TEST_EXPECT(!barwire->pullup_pulldown);
    TEST_EXPECT(true == barwire->exported);
    TEST_EXPECT(true == barwire->external_source);
}

/**
 * Simple component with assignment of simple expression.
 */
TEST(component_with_config_assignment_simple_expression)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    config["propagation_delay"] := 0.1
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(1 == barcomp->config_map.size());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER ==
        barcomp->config_map["propagation_delay"]->type());
    TEST_EXPECT(string("0.1") ==
        barcomp->config_map["propagation_delay"]->eval());
}

/**
 * Simple component with assignment of a nanoseconds value.
 */
TEST(component_with_config_assignment_ns_expression)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    config["propagation_delay"] := ns(27)
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(1 == barcomp->config_map.size());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER ==
        barcomp->config_map["propagation_delay"]->type());
    TEST_EXPECT(string("2.7e-08") ==
        barcomp->config_map["propagation_delay"]->eval());
}

/**
 * Simple component with assignment of a microseconds value.
 */
TEST(component_with_config_assignment_us_expression)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    config["propagation_delay"] := us(27)
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(1 == barcomp->config_map.size());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER ==
        barcomp->config_map["propagation_delay"]->type());
    TEST_EXPECT(string("2.7e-05") ==
        barcomp->config_map["propagation_delay"]->eval());
}

/**
 * Simple component with assignment of a milliseconds value.
 */
TEST(component_with_config_assignment_ms_expression)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    config["propagation_delay"] := ms(27)
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(1 == barcomp->config_map.size());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER ==
        barcomp->config_map["propagation_delay"]->type());
    TEST_EXPECT(string("0.027") ==
        barcomp->config_map["propagation_delay"]->eval());
}

/**
 * Simple component with assignment of a kohms value.
 */
TEST(component_with_config_assignment_kohms_expression)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    config["internal_resistance"] := kohms(10)
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(1 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barcomp = res->component_map["bar"];
    TEST_ASSERT(!!barcomp);
    TEST_EXPECT(string("bar") == barcomp->name);
    TEST_ASSERT(1 == barcomp->config_map.size());
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER ==
        barcomp->config_map["internal_resistance"]->type());
    TEST_EXPECT(string("10000") ==
        barcomp->config_map["internal_resistance"]->eval());
}

/**
 * A wire with a pin assignment can be parsed.
 */
TEST(wire_with_pin_assignment)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bus0 {
                    transceiver0.pin["d1"]
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bus0"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bus0") == barwire->name);
    TEST_ASSERT(1 == barwire->connection_list.size());
    auto conn = barwire->connection_list.front();
    TEST_ASSERT(!!conn);
    TEST_EXPECT(string("transceiver0") == conn->component);
    TEST_EXPECT(string("d1") == conn->pin);
    TEST_EXPECT(!barwire->pullup_pulldown);
}

/**
 * A wire with a pullup can be parsed.
 */
TEST(wire_with_pullup)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bus0 {
                    pullup {
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bus0"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bus0") == barwire->name);
    TEST_ASSERT(0 == barwire->connection_list.size());
    TEST_ASSERT(!!barwire->pullup_pulldown);
    TEST_ASSERT(!!barwire->pullup_pulldown->type);
    TEST_ASSERT(string("pullup") == *barwire->pullup_pulldown->type);
    TEST_EXPECT(0 == barwire->pullup_pulldown->config_map.size());
}

/**
 * A wire with a pullup with config can be parsed.
 */
TEST(wire_with_pullup_with_config)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bus0 {
                    pullup {
                        config["r"] := kohms(10)
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bus0"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bus0") == barwire->name);
    TEST_ASSERT(0 == barwire->connection_list.size());
    TEST_ASSERT(!!barwire->pullup_pulldown);
    TEST_ASSERT(!!barwire->pullup_pulldown->type);
    TEST_ASSERT(string("pullup") == *barwire->pullup_pulldown->type);
    TEST_EXPECT(1 == barwire->pullup_pulldown->config_map.size());
    auto rconfig = barwire->pullup_pulldown->config_map["r"];
    TEST_ASSERT(!!rconfig);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == rconfig->type());
    TEST_EXPECT("10000" == rconfig->eval());
}

/**
 * A wire with a pulldown can be parsed.
 */
TEST(wire_with_pulldown)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bus0 {
                    pulldown {
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bus0"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bus0") == barwire->name);
    TEST_ASSERT(0 == barwire->connection_list.size());
    TEST_ASSERT(!!barwire->pullup_pulldown);
    TEST_ASSERT(!!barwire->pullup_pulldown->type);
    TEST_ASSERT(string("pulldown") == *barwire->pullup_pulldown->type);
    TEST_EXPECT(0 == barwire->pullup_pulldown->config_map.size());
}

/**
 * A wire with a pulldown with config can be parsed.
 */
TEST(wire_with_pulldown_with_config)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bus0 {
                    pulldown {
                        config["r"] := kohms(10)
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(1 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine components. */
    auto barwire = res->wire_map["bus0"];
    TEST_ASSERT(!!barwire);
    TEST_EXPECT(string("bus0") == barwire->name);
    TEST_ASSERT(0 == barwire->connection_list.size());
    TEST_ASSERT(!!barwire->pullup_pulldown);
    TEST_ASSERT(!!barwire->pullup_pulldown->type);
    TEST_ASSERT(string("pulldown") == *barwire->pullup_pulldown->type);
    TEST_EXPECT(1 == barwire->pullup_pulldown->config_map.size());
    auto rconfig = barwire->pullup_pulldown->config_map["r"];
    TEST_ASSERT(!!rconfig);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == rconfig->type());
    TEST_EXPECT("10000" == rconfig->eval());
}

/**
 * A signal probe can be parsed.
 */
TEST(signal_probe)
{
    stringstream in(
        R"TEST(
            module foo {
                signal probe bar {
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("signal") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(0 == barprobe->wire_ref_list.size());
}

/**
 * A signal probe with an id wire can be parsed.
 */
TEST(signal_probe_with_id_wire)
{
    stringstream in(
        R"TEST(
            module foo {
                signal probe bar {
                    wire baz
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("signal") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(1 == barprobe->wire_ref_list.size());
    auto bazwireref = barprobe->wire_ref_list.front();
    TEST_EXPECT(string("baz") == bazwireref);
}

/**
 * A signal probe with a wildcard wire can be parsed.
 */
TEST(signal_probe_with_wildcard_wire)
{
    stringstream in(
        R"TEST(
            module foo {
                signal probe bar {
                    wire *
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("signal") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(1 == barprobe->wire_ref_list.size());
    auto bazwireref = barprobe->wire_ref_list.front();
    TEST_EXPECT(string("*") == bazwireref);
}

/**
 * A signal probe with a type can be parsed.
 */
TEST(signal_probe_with_type)
{
    stringstream in(
        R"TEST(
            module foo {
                signal probe bar {
                    type graph
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("signal") == barprobe->type);
    TEST_ASSERT(!!barprobe->sub_type);
    TEST_EXPECT(string("graph") == *barprobe->sub_type);
    TEST_EXPECT(0 == barprobe->wire_ref_list.size());
}

/**
 * A state probe can be parsed.
 */
TEST(state_probe)
{
    stringstream in(
        R"TEST(
            module foo {
                state probe bar {
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("state") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(0 == barprobe->wire_ref_list.size());
}

/**
 * A state probe with an id wire can be parsed.
 */
TEST(state_probe_with_id_wire)
{
    stringstream in(
        R"TEST(
            module foo {
                state probe bar {
                    wire baz
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("state") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(1 == barprobe->wire_ref_list.size());
    auto bazwireref = barprobe->wire_ref_list.front();
    TEST_EXPECT(string("baz") == bazwireref);
}

/**
 * A state probe with a wildcard wire can be parsed.
 */
TEST(state_probe_with_wildcard_wire)
{
    stringstream in(
        R"TEST(
            module foo {
                state probe bar {
                    wire *
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("state") == barprobe->type);
    TEST_EXPECT(!barprobe->sub_type);
    TEST_EXPECT(1 == barprobe->wire_ref_list.size());
    auto bazwireref = barprobe->wire_ref_list.front();
    TEST_EXPECT(string("*") == bazwireref);
}

/**
 * A state probe with a type can be parsed.
 */
TEST(state_probe_with_type)
{
    stringstream in(
        R"TEST(
            module foo {
                state probe bar {
                    type drc
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(1 == res->probe_map.size());
    TEST_EXPECT(0 == res->scenario_map.size());

    /* examine probes. */
    auto barprobe = res->probe_map["bar"];
    TEST_ASSERT(!!barprobe);
    TEST_EXPECT(string("bar") == barprobe->name);
    TEST_EXPECT(string("state") == barprobe->type);
    TEST_ASSERT(!!barprobe->sub_type);
    TEST_EXPECT(string("drc") == *barprobe->sub_type);
    TEST_EXPECT(0 == barprobe->wire_ref_list.size());
}

/**
 * A scenario can be parsed.
 */
TEST(simple_scenario)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(0 == scenariox->execution_map.size());
}

/**
 * A scenario with an execution and id can be parsed.
 */
TEST(simple_scenario_with_execution_and_id)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution y {
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["y"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("y") == exe->name);
    TEST_EXPECT(0 == exe->step_list.size());
}

/**
 * A scenario with an execution and number can be parsed.
 */
TEST(simple_scenario_with_execution_and_number)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(0 == exe->step_list.size());
}

/**
 * An execution with an "at start" step can be parsed.
 */
TEST(execution_with_step_at_start)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                        at start {
                        }
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(1 == exe->step_list.size());
    auto step = exe->step_list.front();
    TEST_ASSERT(!!step);
    TEST_EXPECT(string("at") == step->type);
    TEST_ASSERT(!!step->step_expression);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == step->step_expression->type());
    TEST_EXPECT(string("0") == step->step_expression->eval());
}

/**
 * An "at start" step with pin assignments can be parsed.
 */
TEST(step_at_start_with_pin_assignment)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                        at start {
                            gate1.pin["a"] := true
                        }
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(1 == exe->step_list.size());
    auto step = exe->step_list.front();
    TEST_ASSERT(!!step);
    TEST_EXPECT(string("at") == step->type);
    TEST_ASSERT(!!step->step_expression);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == step->step_expression->type());
    TEST_EXPECT(string("0") == step->step_expression->eval());
    TEST_EXPECT(1 == step->pin_assignments.size());
    auto assign = step->pin_assignments.front();
    TEST_ASSERT(!!assign);    
    TEST_EXPECT(string("gate1") == assign->lhs_major);
    TEST_EXPECT(string("a") == assign->lhs_minor);
    TEST_ASSERT(!!assign->rhs);
    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_TRUE == assign->rhs->type());
    TEST_EXPECT(string("true") == assign->rhs->eval());
}

/**
 * An execution with an "after ns(27)" step can be parsed.
 */
TEST(execution_with_step_after_ns_27)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                        after ns(27) {
                        }
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(1 == exe->step_list.size());
    auto step = exe->step_list.front();
    TEST_ASSERT(!!step);
    TEST_EXPECT(string("after") == step->type);
    TEST_ASSERT(!!step->step_expression);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == step->step_expression->type());
    TEST_EXPECT(string("2.7e-08") == step->step_expression->eval());
}

/**
 * An "after ns(27)" step with an assertion can be parsed.
 */
TEST(step_after_ns_27_with_assertion)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                        after ns(27) {
                            assert wire.out1.signal = false
                        }
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(1 == exe->step_list.size());
    auto step = exe->step_list.front();
    TEST_ASSERT(!!step);
    TEST_EXPECT(string("after") == step->type);
    TEST_ASSERT(!!step->step_expression);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == step->step_expression->type());
    TEST_EXPECT(string("2.7e-08") == step->step_expression->eval());
    TEST_EXPECT(1 == step->assertion_list.size());
    auto assertion = step->assertion_list.front();
    TEST_ASSERT(!!assertion);
    TEST_EXPECT(string("assert") == assertion->type);
    TEST_EXPECT(string("out1") == assertion->lhs.front());
    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_FALSE == assertion->rhs->type());
    TEST_EXPECT(string("false") == assertion->rhs->eval());
}

/**
 * An "after ns(27)" step with an expectation can be parsed.
 */
TEST(step_after_ns_27_with_expectation)
{
    stringstream in(
        R"TEST(
            module foo {
                scenario x {
                    execution 1 {
                        after ns(27) {
                            expect wire.out1.signal = true
                        }
                    }
                }
            }
        )TEST");
    parser p(in);

    auto res = p.parse();

    TEST_EXPECT(string("foo") == res->name);
    TEST_EXPECT(0 == res->component_map.size());
    TEST_EXPECT(0 == res->wire_map.size());
    TEST_EXPECT(0 == res->probe_map.size());
    TEST_EXPECT(1 == res->scenario_map.size());

    /* examine scenarios. */
    auto scenariox = res->scenario_map["x"];
    TEST_ASSERT(!!scenariox);
    TEST_EXPECT(string("x") == scenariox->name);
    TEST_EXPECT(1 == scenariox->execution_map.size());
    auto exe = scenariox->execution_map["1"];
    TEST_ASSERT(!!exe);
    TEST_EXPECT(string("1") == exe->name);
    TEST_EXPECT(1 == exe->step_list.size());
    auto step = exe->step_list.front();
    TEST_ASSERT(!!step);
    TEST_EXPECT(string("after") == step->type);
    TEST_ASSERT(!!step->step_expression);
    TEST_EXPECT(HOMESIM_TOKEN_NUMBER == step->step_expression->type());
    TEST_EXPECT(string("2.7e-08") == step->step_expression->eval());
    TEST_EXPECT(1 == step->assertion_list.size());
    auto assertion = step->assertion_list.front();
    TEST_ASSERT(!!assertion);
    TEST_EXPECT(string("expect") == assertion->type);
    TEST_EXPECT(string("out1") == assertion->lhs.front());
    TEST_EXPECT(HOMESIM_TOKEN_KEYWORD_TRUE == assertion->rhs->type());
    TEST_EXPECT(string("true") == assertion->rhs->eval());
}
