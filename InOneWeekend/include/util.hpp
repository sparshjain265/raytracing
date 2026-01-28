/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_UTIL_HPP
#define INONEWEEKEND_INCLUDE_UTIL_HPP

#include <concepts>
#include <limits>
#include <numbers>

template <std::floating_point T = double>
inline constexpr T infinity = std::numeric_limits<T>::infinity();

template <std::floating_point T = double>
inline constexpr T pi = static_cast<T>(std::numbers::pi);

template <std::floating_point T = double>
inline constexpr T degreesToRadians(T degrees)
{
    return degrees * (pi<T> / static_cast<T>(180.0));
}

#endif /* INONEWEEKEND_INCLUDE_UTIL_HPP */
