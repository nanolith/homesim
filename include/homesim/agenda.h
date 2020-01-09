/**
 * \file homesim/agenda.h
 *
 * \brief Declarations for the agenda class, which schedules updates to the
 * simulation.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_AGENDA_HEADER_GUARD
# define HOMESIM_AGENDA_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <queue>
#include <vector>

namespace homesim {

/**
 * \brief The agenda class schedules updates to the simulation and maintains a
 * simulation clock.
 */
class agenda
{
public:

    /**
     * \brief Create an agenda instance.
     */
    agenda();

    /**
     * \brief Get the current time.
     *
     * \returns the current time in seconds.
     */
    double current_time() const;

    /**
     * \brief Get the next action to be performed according to the simulation
     * schedule.
     *
     * \returns a pair of bool and function.
     *      - if the Boolean value is true, this is a valid action.
     *      - if the Boolean value is false, there are no more actions on the
     *        agenda.
    */
    std::pair<bool, std::function<void ()>> next();

    /**
     * \brief Pop the top item off of the agenda queue.
     */
    void pop();

    /**
     * \brief Add an action to the agenda, to occur after the given delay.
     *
     * \param delay         The delay in seconds.
     * \param action        The action to occur.
     */
    void add(double delay, std::function<void ()> action);

    /**
     * \brief Clear the agenda and reset the time to 0.
     */
    void clear();

private:

    /**
     * \brief The value stored on the priority queue is a pair of time and
     * action.
     */
    typedef
    std::pair<double, std::function<void ()>>
    time_action;

    /**
     * \brief Compare two time_action elements.
     */
    struct compare_time_action
    {
        inline bool operator ()(time_action lhs, time_action rhs)
        {
            return
                lhs.first > rhs.first;
        }
    };

    /**
     * \brief The type of the priority queue used for tracking the agenda.
     */
    typedef
    std::priority_queue<
        time_action, std::vector<time_action>, compare_time_action>
    queue_type;

    queue_type queue;
    double time;
};

/**
 * \brief The global agenda, used for tracking actions to be performed in a
 * simulation.
 */
extern agenda global_agenda;

/**
 * \brief Propagate all outstanding actions in the simulation until the
 * simulation has converged.
 */
void propagate();

} /* namespace homesim */

#endif /*HOMESIM_AGENDA_HEADER_GUARD*/
