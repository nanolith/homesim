/**
 * \file test/test_component.h
 *
 * \brief Simple test component.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_TEST_COMPONENT_HEADER_GUARD
# define HOMESIM_TEST_COMPONENT_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <homesim/component.h>

namespace homesim {

class test_component : public component
{
public:
    test_component()
        : component{"1a", "1b", "1y", "2a", "2b", "2y"}
    {
    }

    virtual void build() override
    {
        /* force an exception if any of the pins are unbound. */
        for (int i = 0; i < pins(); ++i)
            get_wire(pin_name(i));
    }
};

} /* namespace homesim */

#endif /*HOMESIM_COMPONENT_HEADER_GUARD*/
