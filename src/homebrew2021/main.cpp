/**
 * \file main.cpp
 *
 * \brief Main entry point for homebrew2021.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */

#include <cassert>
#include <homesim/wire.h>
#include <iostream>

#include "alu.h"
#include "basic_register.h"
#include "bus_register.h"
#include "data_bus.h"

using namespace homebrew2021;
using namespace homesim;
using namespace std;

/* forward decls. */
static void
verify_register(
    bus_register* reg, data_bus* bus, wire* clock, wire* clear, wire* read,
    wire* write);
static void
verify_alu_rom(
    shared_ptr<alu_rom_bytes> rom);
static void
verify_basic_register();

int main(int argc, char* argv[])
{
    /* verify that the basic register works. */
    verify_basic_register();

    /* verify that the ALU ROM is correct. */
    verify_alu_rom(get_or_create_alu_rom());

    /* create the data bus. */
    auto dbus = make_shared<data_bus>();

    /* create the clock wire. */
    shared_ptr<wire> clock = make_shared<wire>();
    clock->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    clock->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clock->set_signal(false);

    /* create the clear_a control wire. */
    shared_ptr<wire> ctrl_clr_a = make_shared<wire>();
    ctrl_clr_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_clr_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_clr_a->set_signal(false);

    /* create the read a control wire. */
    shared_ptr<wire> ctrl_read_a = make_shared<wire>();
    ctrl_read_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_read_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_read_a->set_signal(false);

    /* create the write a control wire. */
    shared_ptr<wire> ctrl_write_a = make_shared<wire>();
    ctrl_write_a->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_write_a->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_write_a->set_signal(false);

    /* create the clear_b control wire. */
    shared_ptr<wire> ctrl_clr_b = make_shared<wire>();
    ctrl_clr_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_clr_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_clr_b->set_signal(false);

    /* create the read b control wire. */
    shared_ptr<wire> ctrl_read_b = make_shared<wire>();
    ctrl_read_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_read_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_read_b->set_signal(false);

    /* create the write b control wire. */
    shared_ptr<wire> ctrl_write_b = make_shared<wire>();
    ctrl_write_b->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_write_b->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_write_b->set_signal(false);

    /* create the clear_flags control wire. */
    shared_ptr<wire> ctrl_clr_flags = make_shared<wire>();
    ctrl_clr_flags->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_clr_flags->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_clr_flags->set_signal(false);

    /* create the read flags control wire. */
    shared_ptr<wire> ctrl_read_flags = make_shared<wire>();
    ctrl_read_flags->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_read_flags->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_read_flags->set_signal(false);

    /* create the write flags control wire. */
    shared_ptr<wire> ctrl_write_flags = make_shared<wire>();
    ctrl_write_flags->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    ctrl_write_flags->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    ctrl_write_flags->set_signal(false);

    /* create the A register. */
    auto areg =
        make_shared<bus_register>(
            dbus.get(), clock.get(), ctrl_clr_a.get(), ctrl_read_a.get(),
            ctrl_write_a.get());

    /* create the B register. */
    auto breg =
        make_shared<bus_register>(
            dbus.get(), clock.get(), ctrl_clr_b.get(), ctrl_read_b.get(),
            ctrl_write_b.get());

    /* create the flags register. */
    auto flagsreg =
        make_shared<bus_register>(
            dbus.get(), clock.get(), ctrl_clr_flags.get(),
            ctrl_read_flags.get(), ctrl_write_flags.get());

    /* before starting, propagate... */
    propagate();

    /* verify the a register. */
    verify_register(
        areg.get(), dbus.get(), clock.get(), ctrl_clr_a.get(),
        ctrl_read_a.get(), ctrl_write_a.get());

    /* verify the b register. */
    verify_register(
        breg.get(), dbus.get(), clock.get(), ctrl_clr_b.get(),
        ctrl_read_b.get(), ctrl_write_b.get());

    /* verify the flags register. */
    verify_register(
        flagsreg.get(), dbus.get(), clock.get(), ctrl_clr_flags.get(),
        ctrl_read_flags.get(), ctrl_write_flags.get());

    return 0;
}

static void
verify_register(
    bus_register* reg, data_bus* bus, wire* clock, wire* clear, wire* read,
    wire* write)
{
    /* the bus should start low. */
    for (int i = 0; i < 8; ++i)
    {
        assert(reg->get_data_wire(i)->get_signal() == false);
        assert(bus->get_wire(i)->get_signal() == false);
    }

    /* write all ones to the register. */
    for (int i = 0; i < 8; ++i)
    {
        bus->get_wire(i)->set_signal(true);
    }

    /* turn on write. */
    write->set_signal(true);
    propagate();

    /* pulse the clock. */
    clock->set_signal(true);
    propagate();

    /* turn off clock. */
    clock->set_signal(false);
    propagate();

    /* turn off write. */
    write->set_signal(false);
    propagate();

    /* turn on read. */
    read->set_signal(true);
    propagate();

    /* the register should be output to the bus. */
    for (int i = 0; i < 8; ++i)
    {
        assert(reg->get_data_wire(i)->get_signal() == true);
        assert(bus->get_wire(i)->get_signal() == true);
    }

    /* turn off read. */
    read->set_signal(false);
    propagate();

    /* turn on clear. */
    clear->set_signal(true);
    propagate();

    /* turn off clear. */
    clear->set_signal(false);
    propagate();

    /* turn on read. */
    read->set_signal(true);
    propagate();

    /* the register should be output to the bus. */
    for (int i = 0; i < 8; ++i)
    {
        assert(reg->get_data_wire(i)->get_signal() == false);
        assert(bus->get_wire(i)->get_signal() == false);
    }

    /* turn off read. */
    read->set_signal(false);
    propagate();
}

#define COMPUTE_ALU_ADDRESS(a, b, carry, op) \
      ((((a) & 0xff) << 14) \
     | (((b) & 0xFF) << 6) \
     | (((op) & 0x1F) << 1) \
     | (((carry) & 0x01)))

static void log_operation(
    uint8_t orig_a, uint8_t orig_b, uint8_t a, uint8_t b, uint8_t f, uint8_t ap,
    uint8_t bp, uint8_t fp, const char* op);
static void verify_alu_add(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_addc(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_sub(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_subb(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_mul(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_div(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_mod(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_shl(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_shr(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_rol(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_ror(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_and(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_or(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_inv(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_xor(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_zero(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_one(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);
static void verify_alu_ff(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry);

static void
verify_alu_rom(
    shared_ptr<alu_rom_bytes> rom)
{
    for (size_t a = 0; a < 256; ++a)
    {
        for (size_t b = 0; b < 256; ++b)
        {
            verify_alu_add(rom, a, b, 0);
            verify_alu_add(rom, a, b, 1);
            verify_alu_addc(rom, a, b, 0);
            verify_alu_addc(rom, a, b, 1);
            verify_alu_sub(rom, a, b, 0);
            verify_alu_sub(rom, a, b, 1);
            verify_alu_subb(rom, a, b, 0);
            verify_alu_subb(rom, a, b, 1);
            verify_alu_mul(rom, a, b, 0);
            verify_alu_mul(rom, a, b, 1);
            verify_alu_div(rom, a, b, 0);
            verify_alu_div(rom, a, b, 1);
            verify_alu_mod(rom, a, b, 0);
            verify_alu_mod(rom, a, b, 1);
            verify_alu_shl(rom, a, b, 0);
            verify_alu_shl(rom, a, b, 1);
            verify_alu_shr(rom, a, b, 0);
            verify_alu_shr(rom, a, b, 1);
            verify_alu_rol(rom, a, b, 0);
            verify_alu_rol(rom, a, b, 1);
            verify_alu_ror(rom, a, b, 0);
            verify_alu_ror(rom, a, b, 1);
            verify_alu_and(rom, a, b, 0);
            verify_alu_and(rom, a, b, 1);
            verify_alu_or(rom, a, b, 0);
            verify_alu_or(rom, a, b, 1);
            verify_alu_inv(rom, a, b, 0);
            verify_alu_inv(rom, a, b, 1);
            verify_alu_xor(rom, a, b, 0);
            verify_alu_xor(rom, a, b, 1);
            verify_alu_zero(rom, a, b, 0);
            verify_alu_zero(rom, a, b, 1);
            verify_alu_one(rom, a, b, 0);
            verify_alu_one(rom, a, b, 1);
            verify_alu_ff(rom, a, b, 0);
            verify_alu_ff(rom, a, b, 1);
        }
    }
}

static void log_operation(
    uint8_t orig_a, uint8_t orig_b, uint8_t a, uint8_t b, uint8_t f, uint8_t ap,
    uint8_t bp, uint8_t fp, const char* op)
{
    cout << (size_t)orig_a << " " << op << " " << (size_t)orig_b << " = ("
         << (size_t)a << "," << (size_t)b << "," << (size_t)f
         << ") expected ("
         << (size_t)ap << "," << (size_t)bp << "," << (size_t)fp
         << ")" << endl;
}

static void verify_alu_add(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_add_result_a = (a + b) & 0xFF;
    size_t expected_add_result_b = b;
    size_t expected_add_result_flags = 0;

    size_t add_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADD));
    size_t add_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADD));
    size_t add_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADD));

    if ((a + b) > 255)
    {
        expected_add_result_flags |= ALU_FLAG_CARRY;
    }

    if ((a + b) == 0)
    {
        expected_add_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_add_result_a & 0x80)
    {
        expected_add_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(add_result_a == expected_add_result_a);
    assert(add_result_b == expected_add_result_b);
    assert(add_result_flags == expected_add_result_flags);
}

static void verify_alu_addc(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_add_result_a = (a + b + carry) & 0xFF;
    size_t expected_add_result_b = b;
    size_t expected_add_result_flags = 0;

    size_t add_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADDC));
    size_t add_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADDC));
    size_t add_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ADDC));

    if ((a + b + carry) > 255)
    {
        expected_add_result_flags |= ALU_FLAG_CARRY;
    }

    if ((a + b + carry) == 0)
    {
        expected_add_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_add_result_a & 0x80)
    {
        expected_add_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(add_result_a == expected_add_result_a);
    assert(add_result_b == expected_add_result_b);
    assert(add_result_flags == expected_add_result_flags);
}

static void verify_alu_sub(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_sub_result_a = (a - b) & 0xFF;
    size_t expected_sub_result_b = b;
    size_t expected_sub_result_flags = 0;

    size_t sub_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUB));
    size_t sub_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUB));
    size_t sub_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUB));

    if ((a - b) > 255)
    {
        expected_sub_result_flags |= ALU_FLAG_CARRY;
    }

    if ((a - b) == 0)
    {
        expected_sub_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_sub_result_a & 0x80)
    {
        expected_sub_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(sub_result_a == expected_sub_result_a);
    assert(sub_result_b == expected_sub_result_b);
    assert(sub_result_flags == expected_sub_result_flags);
}

static void verify_alu_subb(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_subb_result_a = (a - b - carry) & 0xFF;
    size_t expected_subb_result_b = b;
    size_t expected_subb_result_flags = 0;

    size_t subb_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUBB));
    size_t subb_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUBB));
    size_t subb_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SUBB));

    if ((a - b - carry) > 255)
    {
        expected_subb_result_flags |= ALU_FLAG_CARRY;
    }

    if ((a - b - carry) == 0)
    {
        expected_subb_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_subb_result_a & 0x80)
    {
        expected_subb_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(subb_result_a == expected_subb_result_a);
    assert(subb_result_b == expected_subb_result_b);
    assert(subb_result_flags == expected_subb_result_flags);
}

static void verify_alu_mul(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_mul_result_a = (a * b) & 0x00FF;
    size_t expected_mul_result_b = ((a * b) & 0xFF00) >> 8;
    size_t expected_mul_result_flags = 0;

    size_t mul_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MUL));
    size_t mul_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MUL));
    size_t mul_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MUL));

    if ((a * b) > 255)
    {
        expected_mul_result_flags |= ALU_FLAG_CARRY;
    }

    if ((a * b) == 0)
    {
        expected_mul_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_mul_result_b & 0x80)
    {
        expected_mul_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(mul_result_a == expected_mul_result_a);
    assert(mul_result_b == expected_mul_result_b);
    assert(mul_result_flags == expected_mul_result_flags);
}

static void verify_alu_div(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t div_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_DIV));
    size_t div_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_DIV));
    size_t div_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_DIV));

    size_t expected_div_result_a;
    size_t expected_div_result_b;
    size_t expected_div_result_flags;

    if (b != 0)
    {
        expected_div_result_a = (a / b) & 0xFF;
        expected_div_result_b = b & 0xFF;
        expected_div_result_flags = 0;

        if ((a / b) > 255)
        {
            expected_div_result_flags |= ALU_FLAG_CARRY;
        }

        if ((a / b) == 0)
        {
            expected_div_result_flags |= ALU_FLAG_ZERO;
        }

        if (expected_div_result_a & 0x80)
        {
            expected_div_result_flags |= ALU_FLAG_NEGATIVE;
        }
    }
    else
    {
        expected_div_result_a = 0;
        expected_div_result_b = 0;
        expected_div_result_flags = ALU_FLAG_DIVIDE_BY_ZERO;
    }

    assert(div_result_a == expected_div_result_a);
    assert(div_result_b == expected_div_result_b);
    assert(div_result_flags == expected_div_result_flags);
}

static void verify_alu_mod(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t mod_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MOD));
    size_t mod_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MOD));
    size_t mod_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_MOD));

    size_t expected_mod_result_a;
    size_t expected_mod_result_b;
    size_t expected_mod_result_flags;

    if (b != 0)
    {
        expected_mod_result_a = (a % b) & 0xFF;
        expected_mod_result_b = b & 0xFF;
        expected_mod_result_flags = 0;

        if ((a % b) > 255)
        {
            expected_mod_result_flags |= ALU_FLAG_CARRY;
        }

        if ((a % b) == 0)
        {
            expected_mod_result_flags |= ALU_FLAG_ZERO;
        }

        if (expected_mod_result_a & 0x80)
        {
            expected_mod_result_flags |= ALU_FLAG_NEGATIVE;
        }
    }
    else
    {
        expected_mod_result_a = 0;
        expected_mod_result_b = 0;
        expected_mod_result_flags = ALU_FLAG_DIVIDE_BY_ZERO;
    }

    assert(mod_result_a == expected_mod_result_a);
    assert(mod_result_b == expected_mod_result_b);
    assert(mod_result_flags == expected_mod_result_flags);
}

static void verify_alu_shl(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_shl_result_a = ((a << 1) | carry) & 0xFF;
    size_t expected_shl_result_b = b;
    size_t expected_shl_result_flags = 0;

    size_t shl_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHL));
    size_t shl_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHL));
    size_t shl_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHL));

    if (((a << 1) | carry) & 0x100)
    {
        expected_shl_result_flags |= ALU_FLAG_CARRY;
    }

    if (((a << 1) | carry) == 0)
    {
        expected_shl_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_shl_result_a & 0x80)
    {
        expected_shl_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(shl_result_a == expected_shl_result_a);
    assert(shl_result_b == expected_shl_result_b);
    assert(shl_result_flags == expected_shl_result_flags);
}

static void verify_alu_shr(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_shr_result_a = ((a >> 1) | (carry << 7)) & 0xFF;
    size_t expected_shr_result_b = b;
    size_t expected_shr_result_flags = 0;

    size_t shr_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHR));
    size_t shr_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHR));
    size_t shr_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_SHR));

    if (a & 0x01)
    {
        expected_shr_result_flags |= ALU_FLAG_CARRY;
    }

    if (expected_shr_result_a == 0)
    {
        expected_shr_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_shr_result_a & 0x80)
    {
        expected_shr_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(shr_result_a == expected_shr_result_a);
    assert(shr_result_b == expected_shr_result_b);
    assert(shr_result_flags == expected_shr_result_flags);
}

static void verify_alu_rol(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_rol_result_a = (a << 1) & 0xFF;
    size_t expected_rol_result_b = b;
    size_t expected_rol_result_flags = 0;

    size_t rol_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROL));
    size_t rol_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROL));
    size_t rol_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROL));

    if ((a << 1) & 0x100)
    {
        expected_rol_result_a |= 0x01;
    }

    if (expected_rol_result_a == 0)
    {
        expected_rol_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_rol_result_a & 0x80)
    {
        expected_rol_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(rol_result_a == expected_rol_result_a);
    assert(rol_result_b == expected_rol_result_b);
    assert(rol_result_flags == expected_rol_result_flags);
}

static void verify_alu_ror(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_ror_result_a = (a >> 1) & 0xFF;
    size_t expected_ror_result_b = b;
    size_t expected_ror_result_flags = 0;

    size_t ror_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROR));
    size_t ror_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROR));
    size_t ror_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ROR));

    if (a & 0x01)
    {
        expected_ror_result_a |= 0x80;
    }

    if (expected_ror_result_a == 0)
    {
        expected_ror_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_ror_result_a & 0x80)
    {
        expected_ror_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(ror_result_a == expected_ror_result_a);
    assert(ror_result_b == expected_ror_result_b);
    assert(ror_result_flags == expected_ror_result_flags);
}

static void verify_alu_and(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_and_result_a = (a & b) & 0xFF;
    size_t expected_and_result_b = b;
    size_t expected_and_result_flags = 0;

    size_t and_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_AND));
    size_t and_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_AND));
    size_t and_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_AND));

    if ((a & b) == 0)
    {
        expected_and_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_and_result_a & 0x80)
    {
        expected_and_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(and_result_a == expected_and_result_a);
    assert(and_result_b == expected_and_result_b);
    assert(and_result_flags == expected_and_result_flags);
}

static void verify_alu_or(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_or_result_a = (a | b) & 0xFF;
    size_t expected_or_result_b = b;
    size_t expected_or_result_flags = 0;

    size_t or_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_OR));
    size_t or_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_OR));
    size_t or_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_OR));

    if ((a | b) == 0)
    {
        expected_or_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_or_result_a & 0x80)
    {
        expected_or_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(or_result_a == expected_or_result_a);
    assert(or_result_b == expected_or_result_b);
    assert(or_result_flags == expected_or_result_flags);
}

static void verify_alu_inv(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_inv_result_a = (~a) & 0xFF;
    size_t expected_inv_result_b = b;
    size_t expected_inv_result_flags = 0;

    size_t inv_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_INV));
    size_t inv_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_INV));
    size_t inv_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_INV));

    if ((~a) == 0)
    {
        expected_inv_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_inv_result_a & 0x80)
    {
        expected_inv_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(inv_result_a == expected_inv_result_a);
    assert(inv_result_b == expected_inv_result_b);
    assert(inv_result_flags == expected_inv_result_flags);
}

static void verify_alu_xor(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_xor_result_a = (a ^ b) & 0xFF;
    size_t expected_xor_result_b = b;
    size_t expected_xor_result_flags = 0;

    size_t xor_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_XOR));
    size_t xor_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_XOR));
    size_t xor_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_XOR));

    if ((a ^ b) == 0)
    {
        expected_xor_result_flags |= ALU_FLAG_ZERO;
    }

    if (expected_xor_result_a & 0x80)
    {
        expected_xor_result_flags |= ALU_FLAG_NEGATIVE;
    }

    assert(xor_result_a == expected_xor_result_a);
    assert(xor_result_b == expected_xor_result_b);
    assert(xor_result_flags == expected_xor_result_flags);
}

static void verify_alu_zero(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_zero_result_a = 0;
    size_t expected_zero_result_b = b;
    size_t expected_zero_result_flags = ALU_FLAG_ZERO;

    size_t zero_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ZERO));
    size_t zero_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ZERO));
    size_t zero_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ZERO));

    assert(zero_result_a == expected_zero_result_a);
    assert(zero_result_b == expected_zero_result_b);
    assert(zero_result_flags == expected_zero_result_flags);
}

static void verify_alu_one(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_one_result_a = 1;
    size_t expected_one_result_b = b;
    size_t expected_one_result_flags = 0;

    size_t one_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ONE));
    size_t one_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ONE));
    size_t one_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_ONE));

    assert(one_result_a == expected_one_result_a);
    assert(one_result_b == expected_one_result_b);
    assert(one_result_flags == expected_one_result_flags);
}

static void verify_alu_ff(
    shared_ptr<alu_rom_bytes> rom, size_t a, size_t b, size_t carry)
{
    size_t expected_ff_result_a = 0xff;
    size_t expected_ff_result_b = b;
    size_t expected_ff_result_flags = ALU_FLAG_NEGATIVE;

    size_t ff_result_a =
        rom->a_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_FF));
    size_t ff_result_b =
        rom->b_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_FF));
    size_t ff_result_flags =
        rom->flags_rom->at(COMPUTE_ALU_ADDRESS(a, b, carry, ALU_OP_FF));

    assert(ff_result_a == expected_ff_result_a);
    assert(ff_result_b == expected_ff_result_b);
    assert(ff_result_flags == expected_ff_result_flags);
}

static void
verify_basic_register()
{
    /* create the clock. */
    auto clock = make_shared<wire>();
    clock->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    clock->set_signal(false);
    propagate();

    /* create the clear wire. */
    auto clear = make_shared<wire>();
    clear->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    clock->set_signal(false);
    propagate();

    /* create the read wire. */
    auto read = make_shared<wire>();
    read->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    read->set_signal(false);
    propagate();

    /* create the write wire. */
    auto write = make_shared<wire>();
    write->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    write->set_signal(false);
    propagate();

    /* create the input wires. */
    auto in1 = make_shared<wire>();
    in1->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in1->set_signal(false);
    auto in2 = make_shared<wire>();
    in2->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in2->set_signal(false);
    auto in3 = make_shared<wire>();
    in3->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in3->set_signal(false);
    auto in4 = make_shared<wire>();
    in4->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in4->set_signal(false);
    auto in5 = make_shared<wire>();
    in5->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in5->set_signal(false);
    auto in6 = make_shared<wire>();
    in6->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in6->set_signal(false);
    auto in7 = make_shared<wire>();
    in7->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in7->set_signal(false);
    auto in8 = make_shared<wire>();
    in8->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    in8->set_signal(false);
    propagate();

    /* create the output wires. */
    auto out1 = make_shared<wire>();
    out1->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out1->set_signal(false);
    auto out2 = make_shared<wire>();
    out2->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out2->set_signal(false);
    auto out3 = make_shared<wire>();
    out3->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out3->set_signal(false);
    auto out4 = make_shared<wire>();
    out4->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out4->set_signal(false);
    auto out5 = make_shared<wire>();
    out5->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out5->set_signal(false);
    auto out6 = make_shared<wire>();
    out6->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out6->set_signal(false);
    auto out7 = make_shared<wire>();
    out7->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out7->set_signal(false);
    auto out8 = make_shared<wire>();
    out8->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
    out8->set_signal(false);
    propagate();

    /* create a basic register. */
    auto reg = make_shared<basic_register>(
        clock.get(), clear.get(), read.get(), write.get(), in1.get(), in2.get(),
        in3.get(), in4.get(), in5.get(), in6.get(), in7.get(), in8.get(),
        out1.get(), out2.get(), out3.get(), out4.get(), out5.get(), out6.get(),
        out7.get(), out8.get());
    propagate();

    /* write all highs to the register. */
    in1->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in1->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in2->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in2->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in3->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in3->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in4->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in4->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in5->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in5->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in6->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in6->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in7->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in7->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    in8->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    in8->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    write->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    write->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    clock->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    propagate();
    clock->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    propagate();
    clock->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, false);
    propagate();
    write->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, false);
    propagate();

    /* read all highs from the register. */
    read->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    read->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    propagate();
    clock->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    propagate();
    assert(true == out1->get_signal());
    assert(true == out2->get_signal());
    assert(true == out3->get_signal());
    assert(true == out4->get_signal());
    assert(true == out5->get_signal());
    assert(true == out6->get_signal());
    assert(true == out7->get_signal());
    assert(true == out8->get_signal());
    clock->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, false);
    propagate();
    read->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, false);
    propagate();

    /* clear the register. */
    clear->add_connection(WIRE_CONNECTION_TYPE_OUTPUT);
    clear->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, true);
    propagate();
    clear->change_connection_type(
        WIRE_CONNECTION_TYPE_OUTPUT, WIRE_CONNECTION_TYPE_OUTPUT, false);
    propagate();
    assert(false == out1->get_signal());
    assert(false == out2->get_signal());
    assert(false == out3->get_signal());
    assert(false == out4->get_signal());
    assert(false == out5->get_signal());
    assert(false == out6->get_signal());
    assert(false == out7->get_signal());
    assert(false == out8->get_signal());
}
