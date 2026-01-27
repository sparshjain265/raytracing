/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_RAY_HPP
#define INONEWEEKEND_INCLUDE_RAY_HPP

#include "vector3.hpp"
#include <concepts>

template <std::floating_point T = double>
class Ray
{
public:
    constexpr Ray() : Ray(Point3<T>(), Vector3<T>()) {}

    constexpr Ray(const Point3<T> &origin, const Vector3<T> &direction)
        : m_origin(origin), m_direction(direction) {}

    constexpr const Point3<T> &origin() const { return m_origin; }
    constexpr const Vector3<T> &direction() const { return m_direction; }

    constexpr Point3<T> at(T t) const
    {
        return m_origin + t * m_direction;
    }

private:
    Point3<T> m_origin;
    Vector3<T> m_direction;
};

using RayD = Ray<double>;
using RayF = Ray<float>;

#endif /* INONEWEEKEND_INCLUDE_RAY_HPP */
