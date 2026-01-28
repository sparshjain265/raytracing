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
#include <limits>

#include "vector3.hpp"
#include "interval.hpp"

template <std::floating_point T = double>
class Color : public Vector3<T>
{
public:
    using Vector3<T>::Vector3;

    Color(Vector3<T> v) : Vector3<T>(v) {}

    constexpr T r() const { return this->x(); }
    constexpr T g() const { return this->y(); }
    constexpr T b() const { return this->z(); }
};

using ColorD = Color<double>;
using ColorF = Color<float>;

template <std::floating_point T>
inline constexpr T linearToGamma(T value, T gamma)
{
    return value > 0 ? std::pow(value, static_cast<T>(1.0) / gamma) : 0;
}

template <std::floating_point T>
inline constexpr T gammaToLinear(T value, T gamma)
{
    return value > 0 ? std::pow(value, gamma) : 0;
}

template <std::floating_point T>
inline constexpr Color<T> linearToGamma(const Color<T> &color, T gamma)
{
    return Color<T>(linearToGamma(color.r(), gamma),
                    linearToGamma(color.g(), gamma),
                    linearToGamma(color.b(), gamma));
}

template <std::floating_point T>
inline constexpr Color<T> gammaToLinear(const Color<T> &color, T gamma)
{
    return Color<T>(gammaToLinear(color.r(), gamma),
                    gammaToLinear(color.g(), gamma),
                    gammaToLinear(color.b(), gamma));
}

template <std::floating_point T>
inline void writeColor(std::ostream &out, const Color<T> &pixelColor)
{
    // Transform into Gamma Space with gamma = 2.2
    const auto color = linearToGamma(pixelColor, static_cast<T>(2.2));

    // Translate the [0, 1] color values to [0,255]
    constexpr T zero = static_cast<T>(0.0);
    constexpr T almostOne = static_cast<T>(0.999);
    constexpr Interval<T> intensity(zero, almostOne);

    // Write the translated [0,255] value of each color component
    const auto rByte = static_cast<int>(256 * intensity.clamp(color.r()));
    const auto gByte = static_cast<int>(256 * intensity.clamp(color.g()));
    const auto bByte = static_cast<int>(256 * intensity.clamp(color.b()));

    // Write the translated [0,255] value of each color component
    out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif /* INONEWEEKEND_SRC_COLOR_HPP */
