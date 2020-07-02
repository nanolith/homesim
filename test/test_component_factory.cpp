/**
 * \file test/test_component_factory.cpp
 *
 * \brief Unit tests for the component factory.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/component.h>
#include <minunit/minunit.h>
#include "test_component.h"

using namespace homesim;
using namespace std;

TEST_SUITE(component_factory);

/* it is possible to create a component factory instance. */
TEST(constructor)
{
    component_factory f;
}

/* if a component type is not found on create, an unknown_component_error is
 * thrown.
 */
TEST(unknown_component_error)
{
    component_factory f;

    try
    {
        auto c = f.create("foo");

        /* create should have thrown an unknown_component_error. */
        TEST_FAILURE();
    }
    catch (unknown_component_error& e)
    {
        TEST_SUCCESS();
    }
}

/* It should be possible to register a component and create it by type.
 */
TEST(register_and_create)
{
    component_factory f;

    f.register_component("test_component", []() -> shared_ptr<component> {
        return make_shared<test_component>();
    });

    auto c = f.create("test_component");
}
