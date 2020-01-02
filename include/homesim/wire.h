/**
 * \file homesim/wire.h
 *
 * \brief Declarations for the wire simulation.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_WIRE_HEADER_GUARD
# define HOMESIM_WIRE_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <list>

namespace homesim {

/**
 * \brief Types of connections that can be made to a wire.
 */
enum wire_connection_type
{
    /** \brief An input reads a signal from the wire. */
    WIRE_CONNECTION_TYPE_INPUT,
    /** \brief An output writes a signal to the wire. */
    WIRE_CONNECTION_TYPE_OUTPUT,
    /** \brief A pull-down weakly associates the wire with logical false. */
    WIRE_CONNECTION_TYPE_PULL_DOWN,
    /** \brief A pull-up weakly associates the wire with logical true. */
    WIRE_CONNECTION_TYPE_PULL_UP,
    /** \brief A high-Z (high impedance) connection is temporarily not
     * participating with this wire. */
    WIRE_CONNECTION_TYPE_HIGH_Z
};

/**
 * \brief A wire represents a network that connects multiple components
 * together. It has a signal, and can be used to perform basic design rule
 * checking.
 */
class wire
{
public:

    /**
     * \brief Constructor for a wire.
     *
     * This constructor sets the wire to the default state, which is logical
     * false, no connections, and DRC checks (faults and floating) disabled.
     */
    wire();

    /**
     * \brief Add a connection of the given type, enabling DRC checks.
     *
     * This type is used to adjust counters which are used when checking for a
     * floating signal or a DRC fault. Furthermore, it runs a set of state
     * change actions which can be used for probing DRC checks either at
     * configuration time or at runtime for dynamic circuits.
     *
     * \param type      The connection type to be added.
     */
    void add_connection(wire_connection_type type);

    /**
     * \brief Change an existing connection type to a different connection type.
     *
     * Certain components, such as transceiver ICs or microcontrollers, can
     * change their connection types on the fly based on simulated conditions.
     * When this occurs, the simulated component can call this function in order
     * to update the current wire state and to perform runtime DRC checking. If
     * the connection type changes to output, then the signal value is used to
     * determine whether the new signal for the wire is true or false.
     *
     * \param oldty     The old type for this connection.
     * \param newty     The new type for this connection.
     * \param signal    The signal to propagate if connection type is becoming
     *                  an output.
     */
    void change_connection_type(
        wire_connection_type oldty, wire_connection_type newty, bool signal);

    /**
     * \brief Get the current signal value for this wire.
     *
     * \returns the signal value, which is either true or false.
     */
    bool get_signal() const;

    /**
     * \brief Set the signal value for this wire.
     *
     * If the new signal value differs, notify listeners that a change has
     * occurred so it can be propagated in the simulation.
     */
    void set_signal(bool newsignal);

    /**
     * \brief Add an action to occur when the wire signal changes.
     *
     * \param action        The action to perform on signal change.
     */
    void add_action(std::function<void ()> action);

    /**
     * \brief Add an action to occur when the connection level state changes.
     *
     * \param action        The action to perform on state change.
     */
    void add_state_change_action(std::function<void ()> action);

    /**
     * \brief Get the number of input connections associated with this wire.
     *
     * \brief return the number of inputs.
     */
    int get_inputs() const;

    /**
     * \brief Get the number of output connections associated with this wire.
     *
     * \brief return the number of outputs.
     */
    int get_outputs() const;

    /**
     * \brief Get the number of high-Z connections associated with this wire.
     *
     * \brief return the number of high-Z connections.
     */
    int get_high_zs() const;

    /**
     * \brief Get the number of pull-ups associated with this wire.
     *
     * \brief return the number of pull-ups.
     */
    int get_pull_ups() const;

    /**
     * \brief Get the number of pull-downs associated with this wire.
     *
     * \brief return the number of pull-downs.
     */
    int get_pull_downs() const;

    /**
     * \brief Is this wire floating?
     */
    bool is_floating() const;

    /**
     * \brief Does this wire have a DRC fault?
     */
    bool has_fault() const;

private:
    bool signal;
    bool floating;
    bool fault;
    int inputs;
    int outputs;
    int high_zs;
    int pull_downs;
    int pull_ups;

    std::list<std::function<void ()>> actions;
    std::list<std::function<void ()>> state_change_actions;

    /**
     * \brief Perform an adjustment on a connection type counter.
     *
     * \param type          The counter on which this adjustment is performed.
     * \param adjustment    The delta for the adjustment.
     */
    void adjust_connection_type(wire_connection_type type, int adjustment);

    /**
     * \brief Perform a fault check on this wire.
     */
    void fault_check();
};

} /* namespace homesim */

#endif /*HOMESIM_WIRE_HEADER_GUARD*/
