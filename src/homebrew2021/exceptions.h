/**
 * \file exceptions.h
 *
 * \brief Exceptions used by the homebrew 2021 computer.
 *
 * \copyright Copyright 2021 Justin Handville. All rights reserved.
 */
#pragma once

namespace homebrew2021
{

/**
 * \brief This occurs when a wire or component requested by index is out of
 * bounds.
 */
class bounds_error : public std::runtime_error
{
public:
    bounds_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

} /* namespace homebrew2021 */
