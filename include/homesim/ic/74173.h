/**
 * \file homesim/ic/74173.h
 *
 * \brief Declarations for a 74173-like Quad D-type Register.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_IC_74173_HEADER_GUARD
# define HOMESIM_IC_74173_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <homesim/constants.h>
#include <homesim/nand_gate.h>
#include <homesim/wire.h>

namespace homesim {

/**
 * \brief 74173 output change delay.
 *
 * By default, this delay mimics worst-case performance for a 74LS173.
 */
constexpr double ic74173_delay = 23.0 * nanoseconds_to_seconds_scale;

/**
 * \brief The ic74173 simulates a 74173 Quad D-type Register.
 */
class ic74173
{
public:

    /**
     * \brief ic74173 constructor, with parameters in roughly DIP package order.
     *
     * \param m             Gate control input M.
     * \param n             Gate control input N.
     * \param out1q         Output 1Q.
     * \param out2q         Output 2Q.
     * \param out3q         Output 3Q.
     * \param out4q         Output 4Q.
     * \param clk           Clock input.
     * \param clr           Clear input.
     * \param in1d          Input 1D.
     * \param in2d          Input 2D.
     * \param in3d          Input 3D.
     * \param in4d          Input 4D.
     * \param g1            Input data-enable 1.
     * \param g2            Input data-enable 2.
     * \param delay         The optional delay in seconds.
     */
    ic74173(
        wire* m, wire* n, wire* out1q, wire* out2q, wire* out3q,
        wire* out4q, wire* clk, wire* clr, wire* in1d, wire* in2d, wire* in3d,
        wire* in4d, wire* g1, wire* g2, double delay = ic74173_delay);

private:
    bool reg[4];
    wire_connection_type conn_type;
};

} /* namespace homesim */

#endif /*HOMESIM_IC_74173_HEADER_GUARD*/
