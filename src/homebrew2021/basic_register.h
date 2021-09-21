/**
 * \file basic_register.h
 *
 * \brief A general purpose 8-bit register.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#pragma once

#include <homesim/inverter.h>
#include <homesim/wire.h>
#include <homesim/ic/74173.h>
#include <homesim/ic/74245.h>

#include "data_bus.h"

namespace homebrew2021
{

/**
 * \brief A general purpose 8-bit register.
 */
class basic_register
{
public:

    /**
     * \brief Constructor for the basic register.
     */
    basic_register(
        data_bus* bus, homesim::wire* clock, homesim::wire* clear,
        homesim::wire* read, homesim::wire* write);

    /**
     * \brief Get a data wire for the register that can be used for feeding
     * another circuit.
     *
     * \param offset        The wire offset.
     *
     * \returns the wire.
     * \throws bounds_error if an invalid offset is requested.
     */
    homesim::wire* get_data_wire(int offset);

private:
    std::shared_ptr<homesim::ic74173> reg[2];
    std::shared_ptr<homesim::ic74245> transceiver;
    std::shared_ptr<homesim::inverter> read_inv;
    std::shared_ptr<homesim::inverter> write_inv;
    std::shared_ptr<homesim::wire> read_wire;
    std::shared_ptr<homesim::wire> write_wire;
    std::shared_ptr<homesim::wire> low_wire;
    std::shared_ptr<homesim::wire> high_wire;
    std::vector<std::shared_ptr<homesim::wire>> data_wires;
};

} /* namespace homebrew2021 */
