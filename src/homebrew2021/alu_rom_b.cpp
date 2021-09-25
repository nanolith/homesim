/**
 * \file alu_rom_b.cpp
 *
 * \brief Machine model for the ALU's B ROM.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include "alu.h"

using namespace homesim;
using namespace homebrew2021;
using namespace std;

/**
 * \brief Create the B ROM for the ALU from output wires, a vector of
 * address wires, and a vector of bytes.
 *
 * \param out1                      Output wire 1.
 * \param out2                      Output wire 2.
 * \param out3                      Output wire 3.
 * \param out4                      Output wire 4.
 * \param out5                      Output wire 5.
 * \param out6                      Output wire 6.
 * \param out7                      Output wire 7.
 * \param out8                      Output wire 8.
 * \param oe                        Output enable wire.
 * \param addrs                     Vector of address wires.
 */
homebrew2021::alu_rom_b::alu_rom_b(
    wire* out1, wire* out2, wire* out3, wire* out4, wire* out5, wire* out6,
    wire* out7, wire* out8, wire* oe, const vector<wire*>& addrs)
{
    auto alu_rom = get_or_create_alu_rom();

    rom =
        make_shared<icrom>(
            addrs, *alu_rom->b_rom, oe, oe,
            out1, out2, out3, out4, out5, out6, out7, out8);
}
