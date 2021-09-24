/**
 * \file alu_get_or_create_alu_rom.cpp
 *
 * \brief Get or create the ALU ROM.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include "alu.h"

using namespace homesim;
using namespace homebrew2021;
using namespace std;

static shared_ptr<alu_rom_bytes> global_rom;

static void populate_alu_rom(shared_ptr<alu_rom_bytes> rom);
static void populate_add_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_addc_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_sub_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_subb_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_mul_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_div_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_mod_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_shl_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_shr_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_rol_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_ror_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_and_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_or_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_inv_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_xor_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_zero_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_one_operations(shared_ptr<alu_rom_bytes> rom);
static void populate_ff_operations(shared_ptr<alu_rom_bytes> rom);

shared_ptr<alu_rom_bytes> homebrew2021::get_or_create_alu_rom()
{
    if (!!global_rom)
    {
        return global_rom;
    }
    else
    {
        global_rom = make_shared<alu_rom_bytes>();
        global_rom->a_rom = make_shared<vector<uint8_t>>(ALU_ROM_SIZE, 0);
        global_rom->b_rom = make_shared<vector<uint8_t>>(ALU_ROM_SIZE, 0);
        global_rom->flags_rom = make_shared<vector<uint8_t>>(ALU_ROM_SIZE, 0);

        populate_alu_rom(global_rom);

        return global_rom;
    }
}

static void populate_alu_rom(shared_ptr<alu_rom_bytes> rom)
{
    populate_add_operations(rom);
    populate_addc_operations(rom);
    populate_sub_operations(rom);
    populate_subb_operations(rom);
    populate_mul_operations(rom);
    populate_div_operations(rom);
    populate_mod_operations(rom);
    populate_shl_operations(rom);
    populate_shr_operations(rom);
    populate_rol_operations(rom);
    populate_ror_operations(rom);
    populate_and_operations(rom);
    populate_or_operations(rom);
    populate_inv_operations(rom);
    populate_xor_operations(rom);
    populate_zero_operations(rom);
    populate_one_operations(rom);
    populate_ff_operations(rom);
}

static void populate_add_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t add_flags = 0;

            if (a + b > 255)
                add_flags |= ALU_FLAG_CARRY;

            if (a + b == 0)
                add_flags |= ALU_FLAG_ZERO;

            if ((a + b) & 0x80)
                add_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ADD, 0)] =
                (uint8_t)(a + b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ADD, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ADD, 0)] =
                add_flags;

            /* carry flag 1 mapping (carry ignored with normal ADD). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ADD, 1)] =
                (uint8_t)(a + b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ADD, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ADD, 1)] =
                add_flags;
        }
    }
}

static void populate_addc_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t add_flags = 0;
            uint8_t addc_flags = 0;

            if (a + b > 255)
                add_flags |= ALU_FLAG_CARRY;

            if (a + b == 0)
                add_flags |= ALU_FLAG_ZERO;

            if ((a + b) & 0x80)
                add_flags |= ALU_FLAG_NEGATIVE;

            if (a + b + 1 > 255)
                addc_flags |= ALU_FLAG_CARRY;

            if (a + b + 1 == 0)
                addc_flags |= ALU_FLAG_ZERO;

            if ((a + b + 1) & 0x80)
                addc_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 0)] =
                (uint8_t)(a + b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 0)] =
                add_flags;

            /* carry flag 1 mapping (carry ignored with normal ADD). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 1)] =
                (uint8_t)(a + b + 1);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ADDC, 1)] =
                addc_flags;
        }
    }
}

static void populate_sub_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t sub_flags = 0;

            if (a - b > 255)
                sub_flags |= ALU_FLAG_CARRY;

            if (a - b == 0)
                sub_flags |= ALU_FLAG_ZERO;

            if ((a - b) & 0x80)
                sub_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SUB, 0)] =
                (uint8_t)(a - b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SUB, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SUB, 0)] =
                sub_flags;

            /* carry flag 1 mapping (carry ignored with normal SUB). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SUB, 1)] =
                (uint8_t)(a - b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SUB, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SUB, 1)] =
                sub_flags;
        }
    }
}

static void populate_subb_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t subb_flags = 0;
            uint8_t subb_carry_flags = 0;

            if (a - b > 255)
                subb_flags |= ALU_FLAG_CARRY;

            if (a - b == 0)
                subb_flags |= ALU_FLAG_ZERO;

            if ((a - b) & 0x80)
                subb_flags |= ALU_FLAG_NEGATIVE;

            if (a - b - 1 > 255)
                subb_carry_flags |= ALU_FLAG_CARRY;

            if (a - b - 1 == 0)
                subb_carry_flags |= ALU_FLAG_ZERO;

            if ((a - b - 1) & 0x80)
                subb_carry_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 0)] =
                (uint8_t)(a - b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 0)] =
                subb_flags;

            /* carry flag 1 mapping (carry ignored with normal SUB). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 1)] =
                (uint8_t)(a - b - 1);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SUBB, 1)] =
                subb_carry_flags;
        }
    }
}

static void populate_mul_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t mul_flags = 0;

            if (a * b > 255)
                mul_flags |= ALU_FLAG_CARRY;

            if (a * b == 0)
                mul_flags |= ALU_FLAG_ZERO;

            if ((a * b) & 0x8000)
                mul_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_MUL, 0)] =
                (uint8_t)(a * b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_MUL, 0)] =
                (uint8_t)((a * b) >> 8);
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_MUL, 0)] =
                mul_flags;

            /* carry flag 1 mapping (carry ignored with normal MUL). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_MUL, 1)] =
                (uint8_t)(a * b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_MUL, 1)] =
                (uint8_t)((a * b) >> 8);
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_MUL, 1)] =
                mul_flags;
        }
    }
}

static void populate_div_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 1; b < 256; ++b)
        {
            uint8_t div_flags = 0;

            if (a / b > 255)
                div_flags |= ALU_FLAG_CARRY;

            if (a / b == 0)
                div_flags |= ALU_FLAG_ZERO;

            if ((a / b) & 0x80)
                div_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_DIV, 0)] =
                (uint8_t)(a / b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_DIV, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_DIV, 0)] =
                div_flags;

            /* carry flag 1 mapping (carry ignored with normal DIV). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_DIV, 1)] =
                (uint8_t)(a / b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_DIV, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_DIV, 1)] =
                div_flags;
        }

        /* HANDLE SPECIAL DIVIDE BY ZERO CASE */

        /* carry flag 0 mapping. */
        (*rom->a_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 0)] = 0;
        (*rom->b_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 0)] = 0;
        (*rom->flags_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 0)] =
            ALU_FLAG_DIVIDE_BY_ZERO;

        /* carry flag 1 mapping (carry ignored with normal DIV). */
        (*rom->a_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 1)] = 0;
        (*rom->b_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 1)] = 0;
        (*rom->flags_rom)[compute_alu_address(a, 0, ALU_OP_DIV, 1)] =
            ALU_FLAG_DIVIDE_BY_ZERO;
    }
}

static void populate_mod_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 1; b < 256; ++b)
        {
            uint8_t mod_flags = 0;

            if (a % b > 255)
                mod_flags |= ALU_FLAG_CARRY;

            if (a % b == 0)
                mod_flags |= ALU_FLAG_ZERO;

            if ((a % b) & 0x80)
                mod_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_MOD, 0)] =
                (uint8_t)(a % b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_MOD, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_MOD, 0)] =
                mod_flags;

            /* carry flag 1 mapping (carry ignored with normal MOD). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_MOD, 1)] =
                (uint8_t)(a % b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_MOD, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_MOD, 1)] =
                mod_flags;
        }

        /* HANDLE SPECIAL DIVIDE BY ZERO CASE */

        /* carry flag 0 mapping. */
        (*rom->a_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 0)] = 0;
        (*rom->b_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 0)] = 0;
        (*rom->flags_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 0)] =
            ALU_FLAG_DIVIDE_BY_ZERO;

        /* carry flag 1 mapping (carry ignored with normal MOD). */
        (*rom->a_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 1)] = 0;
        (*rom->b_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 1)] = 0;
        (*rom->flags_rom)[compute_alu_address(a, 0, ALU_OP_MOD, 1)] =
            ALU_FLAG_DIVIDE_BY_ZERO;
    }
}

static void populate_shl_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t shl_flags = 0;
            uint8_t shl_carry_flags = 0;

            if ((a << 1) & 0x100)
                shl_flags |= ALU_FLAG_CARRY;

            if ((a << 1) == 0)
                shl_flags |= ALU_FLAG_ZERO;

            if ((a << 1) & 0x80)
                shl_flags |= ALU_FLAG_NEGATIVE;

            if (((a << 1) | 1) & 0x100)
                shl_carry_flags |= ALU_FLAG_CARRY;

            if (((a << 1) | 1) == 0)
                shl_carry_flags |= ALU_FLAG_ZERO;

            if (((a << 1) | 1) & 0x80)
                shl_carry_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SHL, 0)] =
                (uint8_t)(a << 1);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SHL, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SHL, 0)] =
                shl_flags;

            /* carry flag 1 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SHL, 1)] =
                (uint8_t)((a << 1) | 1);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SHL, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SHL, 1)] =
                shl_carry_flags;
        }
    }
}

static void populate_shr_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t shr_flags = 0;
            uint8_t shr_carry_flags = 0;

            if (a & 0x01)
                shr_flags |= ALU_FLAG_CARRY;

            if ((a >> 1) == 0)
                shr_flags |= ALU_FLAG_ZERO;

            if ((a >> 1) & 0x80)
                shr_flags |= ALU_FLAG_NEGATIVE;

            if (a & 0x01)
                shr_carry_flags |= ALU_FLAG_CARRY;

            if (((a >> 1) | 0x80) == 0)
                shr_carry_flags |= ALU_FLAG_ZERO;

            if (((a >> 1) | 0x80) & 0x80)
                shr_carry_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SHR, 0)] =
                (uint8_t)(a >> 1);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SHR, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SHR, 0)] =
                shr_flags;

            /* carry flag 1 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_SHR, 1)] =
                (uint8_t)((a >> 1) | 0x80);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_SHR, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_SHR, 1)] =
                shr_carry_flags;
        }
    }
}

static void populate_rol_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t rol_flags = 0;
            uint8_t carry = 0;

            if ((a << 1) & 0x100)
                carry = 1;

            if (((a << 1) | carry) == 0)
                rol_flags |= ALU_FLAG_ZERO;

            if (((a << 1) | carry) & 0x80)
                rol_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ROL, 0)] =
                (uint8_t)((a << 1) | carry);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ROL, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ROL, 0)] =
                rol_flags;

            /* carry flag 1 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ROL, 1)] =
                (uint8_t)((a << 1) | carry);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ROL, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ROL, 1)] =
                rol_flags;
        }
    }
}

static void populate_ror_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t ror_flags = 0;
            uint8_t carry = 0;

            if (a & 0x01)
                carry = 0x80;

            if (((a >> 1) | carry) == 0)
                ror_flags |= ALU_FLAG_ZERO;

            if (((a >> 1) | carry) & 0x80)
                ror_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ROR, 0)] =
                (uint8_t)((a >> 1) | carry);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ROR, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ROR, 0)] =
                ror_flags;

            /* carry flag 1 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ROR, 1)] =
                (uint8_t)((a >> 1) | carry);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ROR, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ROR, 1)] =
                ror_flags;
        }
    }
}

static void populate_and_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t and_flags = 0;

            if ((a & b) == 0)
                and_flags |= ALU_FLAG_ZERO;

            if ((a & b) & 0x80)
                and_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_AND, 0)] =
                (uint8_t)(a & b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_AND, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_AND, 0)] =
                and_flags;

            /* carry flag 1 mapping (carry ignored with AND). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_AND, 1)] =
                (uint8_t)(a & b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_AND, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_AND, 1)] =
                and_flags;
        }
    }
}

static void populate_or_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t or_flags = 0;

            if ((a | b) == 0)
                or_flags |= ALU_FLAG_ZERO;

            if ((a | b) & 0x80)
                or_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_OR, 0)] =
                (uint8_t)(a | b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_OR, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_OR, 0)] =
                or_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_OR, 1)] =
                (uint8_t)(a | b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_OR, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_OR, 1)] =
                or_flags;
        }
    }
}

static void populate_inv_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t inv_flags = 0;

            if ((~a) == 0)
                inv_flags |= ALU_FLAG_ZERO;

            if ((~a) & 0x80)
                inv_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_INV, 0)] =
                (uint8_t)(~a);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_INV, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_INV, 0)] =
                inv_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_INV, 1)] =
                (uint8_t)(~a);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_INV, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_INV, 1)] =
                inv_flags;
        }
    }
}

static void populate_xor_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t xor_flags = 0;

            if ((a ^ b) == 0)
                xor_flags |= ALU_FLAG_ZERO;

            if ((a ^ b) & 0x80)
                xor_flags |= ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_XOR, 0)] =
                (uint8_t)(a ^ b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_XOR, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_XOR, 0)] =
                xor_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_XOR, 1)] =
                (uint8_t)(a ^ b);
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_XOR, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_XOR, 1)] =
                xor_flags;
        }
    }
}

static void populate_zero_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t zero_flags = 0;

            zero_flags |= ALU_FLAG_ZERO;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 0)] = 0;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 0)] =
                zero_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 1)] = 0;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ZERO, 1)] =
                zero_flags;
        }
    }
}

static void populate_one_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t one_flags = 0;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ONE, 0)] = 1;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ONE, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ONE, 0)] =
                one_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_ONE, 1)] = 1;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_ONE, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_ONE, 1)] =
                one_flags;
        }
    }
}

static void populate_ff_operations(shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            uint8_t ff_flags = ALU_FLAG_NEGATIVE;

            /* carry flag 0 mapping. */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_FF, 0)] = 0xff;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_FF, 0)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_FF, 0)] =
                ff_flags;

            /* carry flag 1 mapping (carry ignored with OR). */
            (*rom->a_rom)[compute_alu_address(a, b, ALU_OP_FF, 1)] = 0xff;
            (*rom->b_rom)[compute_alu_address(a, b, ALU_OP_FF, 1)] =
                (uint8_t)b;
            (*rom->flags_rom)[compute_alu_address(a, b, ALU_OP_FF, 1)] =
                ff_flags;
        }
    }
}
