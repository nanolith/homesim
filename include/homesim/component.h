/**
 * \file homesim/component.h
 *
 * \brief Declarations for the component abstraction.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_COMPONENT_HEADER_GUARD
# define HOMESIM_COMPONENT_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <homesim/wire.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace homesim {

/**
 * \brief This exception is thrown when the requested pin name is invalid.
 */
class invalid_pin_error : public std::runtime_error
{
public:
    invalid_pin_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief This exception is thrown when the requested pin name is invalid.
 */
class pin_binding_error : public std::runtime_error
{
public:
    pin_binding_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief This exception is thrown when the requested pin has no wire.
 */
class missing_wire_error : public std::runtime_error
{
public:
    missing_wire_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief The requested component type is unknown.
 */
class unknown_component_error : public std::runtime_error
{
public:
    unknown_component_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief The component abstraction allows gates and components to be treated
 * generically.
 */
class component
{
public:

    /**
     * \brief Set a pin to the given wire.
     *
     * \param name      The name of the pin to set.
     * \param w         The wire to set.
     *
     * \throws
     *      - \ref invalid_pin_error if the pin name is invalid.
     *      - \ref pin_binding_error if the pin is already bound.
     */
    void set_pin(const std::string& name, wire* w);

    /**
     * \brief Get the number of pins for this component.
     */
    int pins() const;

    /**
     * \brief Get the pin name for the given index.
     *
     * \param index     The index of the pin name to get.
     */
    const std::string& pin_name(int index) const;

    /**
     * \brief Build the component instance.
     */
    virtual void build() = 0;

protected:

    /**
     * \brief Constructor for component.
     *
     * \param pins      Initializer list of pins.
     */
    component(std::initializer_list<std::string> pins);

    /**
     * \brief Get the wire for a pin.
     *
     * \param pin_name  The name of the pin to get.
     *
     * \throws \ref missing_wire_error if the wire is missing.
     */
    wire* get_wire(const std::string& pin_name);

private:
    std::map<std::string, wire*> pin_map;
    std::vector<std::string> pin_name_array;
};

/**
 * \brief Build a component by name.
 */
class component_factory
{
public:

    /**
     * \brief Create a component factory instance.
     */
    component_factory();

    /**
     * \brief Register a component constructor by type.
     *
     * \param type      The type of the component to register.
     * \param ctor      The component constructor function to register.
     */
    void register_component(
        const std::string& type,
        std::function<std::shared_ptr<component> ()> ctor);

    /**
     * \brief Create a component by type.
     *
     * \param type      The name of the component to create.
     *
     * \throws \ref unknown_component_error if the component type is unknown.
     */
    std::shared_ptr<component> create(const std::string& type);

private:
    std::map<std::string, std::function<std::shared_ptr<component> ()>>
    component_map;
};

} /* namespace homesim */

#endif /*HOMESIM_COMPONENT_HEADER_GUARD*/
