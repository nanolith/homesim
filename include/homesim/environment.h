/**
 * \file homesim/environment.h
 *
 * \brief Environmental context for the simulator.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_ENVIRONMENT_HEADER_GUARD
# define HOMESIM_ENVIRONMENT_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <memory>

namespace homesim {

class component_factory;

class environment
{
public:

    /**
     * \brief Construct an empty environment.
     */
    environment();

    /**
     * \brief Get the component factory for this environment.
     */
    component_factory& get_component_factory();

private:
    std::shared_ptr<component_factory> cf;
};

} /* namespace homesim */

#endif /*HOMESIM_SEMANTIC_ANALYZER_HEADER_GUARD*/
