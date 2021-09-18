/**
 * \file data_bus.h
 *
 * \brief The data bus for the homebrew2021 computer.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#pragma once

#include <homesim/wire.h>
#include <memory>

#include "exceptions.h"

namespace homebrew2021
{

/**
 * \brief This class implements the 8-bit data bus.
 */
class data_bus
{
public:

    /**
     * \brief Data bus constructor.
     */
    data_bus();

    /**
     * \brief Get a wire reference.
     */
    homesim::wire* get_wire(int offset);

private:
    std::shared_ptr<homesim::wire> bus[8];
};

} /* namespace homebrew2021 */
