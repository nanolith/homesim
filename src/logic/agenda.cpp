/**
 * \file logic/agenda.cpp
 *
 * \brief Agenda constructor and global instance.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/agenda.h>

using namespace homesim;
using namespace std;

/**
 * \brief The global agenda instance.
 */
agenda homesim::global_agenda;

/**
 * \brief Create an agenda instance.
 */
homesim::agenda::agenda()
    : time(0.0)
{
}
