/**
 * \file logic/ic7404.cpp
 *
 * \brief IC 7404 (Hex Inverter gate) constructor.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/ic/7404.h>

using namespace homesim;
using namespace std;

/**
 * \brief ic7404 constructor, with parameters in roughly DIP package order.
 *
 * \param in1           Input for gate 1.
 * \param out1          Output for gate 1.
 * \param in2           Input for gate 2.
 * \param out2          Output for gate 2.
 * \param in3           Input for gate 3.
 * \param out3          Output for gate 3.
 * \param out4          Output for gate 4.
 * \param in4           Input for gate 4.
 * \param out5          Output for gate 5.
 * \param in5           Input for gate 5.
 * \param out6          Output for gate 6.
 * \param in6           Input for gate 6.
 * \param delay         The optional delay in seconds.
 */
homesim::ic7404::ic7404(
    wire* in1, wire* out1, wire* in2, wire* out2, wire* in3, wire* out3,
    wire* out4, wire* in4, wire* out5, wire* in5, wire* out6, wire* in6,
    double delay)
    : g1(in1, out1, delay)
    , g2(in2, out2, delay)
    , g3(in3, out3, delay)
    , g4(in4, out4, delay)
    , g5(in5, out5, delay)
    , g6(in6, out6, delay)
{
}
