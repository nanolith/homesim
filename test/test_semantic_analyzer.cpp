/**
 * \file test/test_semantic_analyzer.cpp
 *
 * \brief Unit tests for the semantic analyzer.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/environment.h>
#include <homesim/semantic_analyzer.h>
#include <iostream>
#include <memory>
#include <minunit/minunit.h>
#include <sstream>
#include "test_component.h"

using namespace homesim;
using namespace std;

TEST_SUITE(semantic_analyzer);

/**
 * A semantic analyzer can be created from an environment and an AST.
 */
TEST(construction)
{
    stringstream in(
        R"TEST(
            module foo {
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    semantic_analyzer analyzer(en, ast);
}

/**
 * A semantic_error occurs when a missing component type is encountered.
 */
TEST(missing_component_type)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * A semantic_error occurs when an unknown component type is encountered.
 */
TEST(unknown_component_type)
{
    stringstream in(
        R"TEST(
            module foo {
                component bar {
                    type baz
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * Two components with the same name cause a semantic error.
 */
TEST(double_component)
{
    stringstream in(
        R"TEST(
            module foo {
                component baz {
                    type test_component
                }
                component baz {
                    type test_component
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    /* register the test component. */
    en->get_component_factory().register_component(
        "test_component", []() -> shared_ptr<component> {
            return make_shared<test_component>();
        });

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * Two wires with the same name cause an error.
 */
TEST(double_wire)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bar { }
                wire bar { }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * A wire referencing an invalid component throws an error.
 */
TEST(unknown_component)
{
    stringstream in(
        R"TEST(
            module foo {
                wire bar {
                    baz.pin["1a"]
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * A wire referencing an invalid pin throws an error.
 */
TEST(unknown_pin)
{
    stringstream in(
        R"TEST(
            module foo {
                component baz {
                    type test_component
                }
                wire bar {
                    baz.pin["unknown"]
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    /* register the test component. */
    en->get_component_factory().register_component(
        "test_component", []() -> shared_ptr<component> {
            return make_shared<test_component>();
        });

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}

/**
 * A pin can only be bound once.
 */
TEST(double_bound_pin)
{
    stringstream in(
        R"TEST(
            module foo {
                component baz {
                    type test_component
                }
                wire bar {
                    baz.pin["1a"]
                    baz.pin["1a"]
                }
            }
        )TEST");
    parser p(in);
    auto ast = p.parse();
    auto en = make_shared<environment>();

    /* register the test component. */
    en->get_component_factory().register_component(
        "test_component", []() -> shared_ptr<component> {
            return make_shared<test_component>();
        });

    semantic_analyzer analyzer(en, ast);

    try
    {
        /* this should throw an exception. */
        auto sim = analyzer.extract_simulation();

        /* if we've made it this far, we've failed. */
        TEST_FAILURE();
    } catch (semantic_error& e)
    {
        TEST_SUCCESS();
    }
}
