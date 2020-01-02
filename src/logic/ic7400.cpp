/**
 * \file logic/ic7400.cpp
 *
 * \brief IC 7400 (Quad NAND gate) constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/ic/7400.h>

using namespace std;

/**
 * \brief ic7402 constructor, with parameters in roughly DIP package order.
 *
 * \param out1y         Output of gate 1.
 * \param in1a          Input A for gate 1.
 * \param in1b          Input B for gate 1.
 * \param out2y         Output of gate 2.
 * \param in2a          Input A for gate 2.
 * \param in2b          Input B for gate 2.
 * \param in3a          Input A for gate 3.
 * \param in3b          Input B for gate 3.
 * \param out3y         Output for gate 3.
 * \param in4a          Input A for gate 4.
 * \param in4b          Input B for gate 4.
 * \param out4y         Output for gate 4.
 * \param delay         The optional delay in seconds.
 */
homesim::ic7400::ic7400(
    wire* in1a, wire* in1b, wire* out1y, wire* in2a, wire* in2b, wire* out2y,
    wire* out3y, wire* in3b, wire* in3a, wire* out4y, wire* in4b, wire* in4a,
    double delay)
        : g1(in1a, in1b, out1y, delay)
        , g2(in2a, in2b, out2y, delay)
        , g3(in3a, in3b, out3y, delay)
        , g4(in4a, in4b, out4y, delay)
{
}
