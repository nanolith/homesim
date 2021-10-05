/**
 * \file basic_register.h
 *
 * \brief A basic 8-bit register, wired however the caller wants it wired.
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
 * \brief A general purpose 8-bit register, wired however the caller wants it
 * wired.
 */
class basic_register
{
public:

    /**
     * \brief Constructor for the basic register.
     */
    basic_register(
        homesim::wire* clock, homesim::wire* clear,
        homesim::wire* read, homesim::wire* write,
        homesim::wire* in1, homesim::wire* in2, homesim::wire* in3,
        homesim::wire* in4, homesim::wire* in5, homesim::wire* in6,
        homesim::wire* in7, homesim::wire* in8,
        homesim::wire* out1, homesim::wire* out2, homesim::wire* out3,
        homesim::wire* out4, homesim::wire* out5, homesim::wire* out6,
        homesim::wire* out7, homesim::wire* out8);

private:
    std::shared_ptr<homesim::ic74173> reg[2];
    std::shared_ptr<homesim::inverter> read_inv;
    std::shared_ptr<homesim::inverter> write_inv;
    std::shared_ptr<homesim::wire> read_wire;
    std::shared_ptr<homesim::wire> write_wire;
};

} /* namespace homebrew2021 */
