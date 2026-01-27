/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_COLOR_HPP
#define INONEWEEKEND_INCLUDE_COLOR_HPP

#include "vector3.hpp"
#include <concepts>
#include <iostream>

template <std::floating_point T = double>
using Color = Vector3<T>;

using ColorD = Color<double>;
using ColorF = Color<float>;

template <std::floating_point T>
inline void writeColor(std::ostream &out, const Color<T> &pixelColor)
{
    // Write the translated [0,255] value of each color component
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Write the translated [0,255] value of each color component
    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

#endif /* INONEWEEKEND_SRC_COLOR_HPP */
