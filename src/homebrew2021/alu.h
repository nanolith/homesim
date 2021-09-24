/**
 * \file alu.h
 *
 * \brief A lookup table based ALU.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#pragma once

#include <homesim/ic/rom.h>
#include <homesim/wire.h>

#include "data_bus.h"

namespace homebrew2021
{

/* XXX - ALU has three registers: A', B', and FLAGS', which can cache output
 * from the mapping operation.  These can then be transferred to the real
 * registers. */

/**
 * \brief The operations supported by the ALU.
 */
enum alu_op
{
    ALU_OP_ADD          =  0,
    ALU_OP_ADDC         =  1,
    ALU_OP_SUB          =  2,
    ALU_OP_SUBB         =  3,
    ALU_OP_MUL          =  4,
    ALU_OP_DIV          =  5,
    ALU_OP_MOD          =  6,
    ALU_OP_SHL          =  7,
    ALU_OP_SHR          =  8,
    ALU_OP_ROL          =  9,
    ALU_OP_ROR          = 10,
    ALU_OP_AND          = 11,
    ALU_OP_OR           = 12,
    ALU_OP_INV          = 13,
    ALU_OP_XOR          = 14,
    ALU_OP_ZERO         = 15,
    ALU_OP_ONE          = 16,
    ALU_OP_FF           = 17,
    ALU_OP_RESERVED_18  = 18,
    ALU_OP_RESERVED_19  = 19,
    ALU_OP_RESERVED_20  = 20,
    ALU_OP_RESERVED_21  = 21,
    ALU_OP_RESERVED_22  = 22,
    ALU_OP_RESERVED_23  = 23,
    ALU_OP_RESERVED_24  = 24,
    ALU_OP_RESERVED_25  = 25,
    ALU_OP_RESERVED_26  = 26,
    ALU_OP_RESERVED_27  = 27,
    ALU_OP_RESERVED_28  = 28,
    ALU_OP_RESERVED_29  = 29,
    ALU_OP_RESERVED_30  = 30,
    ALU_OP_RESERVED_31  = 31,
};

#define ALU_FLAG_ZERO             1
#define ALU_FLAG_CARRY            2
#define ALU_FLAG_NEGATIVE         4
#define ALU_FLAG_DIVIDE_BY_ZERO  64
#define ALU_FLAG_EXCEPTION      128

struct alu_rom_bytes
{
    std::shared_ptr<std::vector<uint8_t>> a_rom;
    std::shared_ptr<std::vector<uint8_t>> b_rom;
    std::shared_ptr<std::vector<uint8_t>> flags_rom;
};

/* the maximum unsigned size of the A register. */
constexpr std::size_t ALU_A_MAX_UNSIGNED_SIZE = 256;

/* the maximum unsigned size of the B register. */
constexpr std::size_t ALU_B_MAX_UNSIGNED_SIZE = 256;

/* the number of mappings of unique A and B combinations. */
constexpr std::size_t ALU_A_B_MAPPING_SIZE =
    ALU_A_MAX_UNSIGNED_SIZE * ALU_B_MAX_UNSIGNED_SIZE;

/* the number of combinations of carry flags. */
constexpr std::size_t ALU_CARRY_MAPPING_SIZE = 2;

/* the number of possible ALU operations. */
constexpr std::size_t ALU_OPS_MAPPING_SIZE = 32;

/* the total combinatorial size of an ALU ROM. */
constexpr std::size_t ALU_ROM_SIZE =
    ALU_A_B_MAPPING_SIZE * ALU_OPS_MAPPING_SIZE * ALU_CARRY_MAPPING_SIZE;

/* verify that our calculations match our assumptions regarding ROM size. */
static_assert(
    ALU_ROM_SIZE == (4UL * 1024UL * 1024UL), "ALU ROM should be 4MB.");

std::shared_ptr<alu_rom_bytes> get_or_create_alu_rom();

constexpr inline std::size_t compute_alu_address(
    std::size_t a, std::size_t b, std::size_t op, std::size_t carry)
{
    return
          ((a & 0xFF) << 14)
        | ((b & 0xFF) << 6)
        | ((op & 0x1F) << 1)
        | ((carry & 0x01));
}

class alu_rom_a
{
    /**
     * \brief Create the A ROM for the ALU from output wires, a vector of
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
     * \param bytes                     Vector of ROM bytes.
     */
    alu_rom_a(
        homesim::wire* out1, homesim::wire* out2, homesim::wire* out3,
        homesim::wire* out4, homesim::wire* out5, homesim::wire* out6,
        homesim::wire* out7, homesim::wire* out8, homesim::wire* oe,
        const std::vector<homesim::wire*>& addrs,
        const std::vector<std::uint8_t>& bytes);

private:
    std::shared_ptr<homesim::icrom> rom;
};

class alu_rom_b
{
public:
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
     * \param bytes                     Vector of ROM bytes.
     */
    alu_rom_b(
        homesim::wire* out1, homesim::wire* out2, homesim::wire* out3,
        homesim::wire* out4, homesim::wire* out5, homesim::wire* out6,
        homesim::wire* out7, homesim::wire* out8, homesim::wire* oe,
        const std::vector<homesim::wire*>& addrs,
        const std::vector<std::uint8_t>& bytes);

private:
    std::shared_ptr<homesim::icrom> rom;
};

class alu_rom_flags
{
public:
    /**
     * \brief Create the FLAGS ROM for the ALU from output wires, a vector of
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
     * \param bytes                     Vector of ROM bytes.
     */
    alu_rom_flags(
        homesim::wire* out1, homesim::wire* out2, homesim::wire* out3,
        homesim::wire* out4, homesim::wire* out5, homesim::wire* out6,
        homesim::wire* out7, homesim::wire* out8, homesim::wire* oe,
        const std::vector<homesim::wire*>& addrs,
        const std::vector<std::uint8_t>& bytes);

private:
    std::shared_ptr<homesim::icrom> rom;
};

} /* namespace homebrew2021 */
