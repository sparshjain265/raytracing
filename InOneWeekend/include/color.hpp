/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_COLOR_HPP
#define INONEWEEKEND_INCLUDE_COLOR_HPP

#include <concepts>
#include <iostream>

#include "vector3.hpp"
#include "interval.hpp"

template <std::floating_point T = double>
using Color = Vector3<T>;

using ColorD = Color<double>;
using ColorF = Color<float>;

template <std::floating_point T>
inline void writeColor(std::ostream &out, const Color<T> &pixelColor)
{
    // Write the translated [0,255] value of each color component
    const auto r = pixelColor.x();
    const auto g = pixelColor.y();
    const auto b = pixelColor.z();

    // Translate the [0, 1] color values to [0,255]
    constexpr Interval<T> intensity(0.0, 0.999);

    const auto rByte = static_cast<int>(256 * intensity.clamp(r));
    const auto gByte = static_cast<int>(256 * intensity.clamp(g));
    const auto bByte = static_cast<int>(256 * intensity.clamp(b));

    // Write the translated [0,255] value of each color component
    out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif /* INONEWEEKEND_SRC_COLOR_HPP */
