/**
 * \file data_bus.cpp
 *
 * \brief data_bus constructor.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#include "data_bus.h"

using namespace homesim;
using namespace std;

homebrew2021::data_bus::data_bus()
{
    for (int i = 0; i < 8; ++i)
    {
        bus[i] = make_shared<wire>();
        bus[i]->add_connection(WIRE_CONNECTION_TYPE_PULL_DOWN);
        bus[i]->set_signal(false);
    }
}
