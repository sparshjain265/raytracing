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

#include "random.hpp"

namespace Util
{

    template <std::floating_point T = double>
    inline constexpr T infinity = std::numeric_limits<T>::infinity();

    template <std::floating_point T = double>
    inline constexpr T pi = static_cast<T>(std::numbers::pi);

    template <std::floating_point T = double>
    inline constexpr T degreesToRadians(T degrees)
    {
        return degrees * (pi<T> / static_cast<T>(180.0));
    }

    template <std::floating_point T = double>
    inline constexpr T radiansToDegrees(T radians)
    {
        return radians * (static_cast<T>(180.0) / pi<T>);
    }

    template <std::floating_point T = double>
    inline T random()
    {
        using _type = std::uint64_t;

        // Returns a random real in [0, 1) using our global std::mt19937 object
        constexpr auto low = static_cast<_type>(0);
        constexpr auto high = std::numeric_limits<_type>::max();
        const auto r = Random::get(low, high - 1);
        return static_cast<T>(r) / static_cast<T>(high);
    }

    template <std::floating_point T = double>
    inline T random(T min, T max)
    {
        // Returns a random real in [min, max) using our global std::mt19937 object
        return min + (max - min) * random<T>();
    }

} // namespace Util

// Expose constants in the global namespace
using Util::infinity;
using Util::pi;

#endif /* INONEWEEKEND_INCLUDE_UTIL_HPP */
