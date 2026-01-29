/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_VECTOR3_HPP
#define INONEWEEKEND_INCLUDE_VECTOR3_HPP

#include <cmath>
#include <iostream>
#include <array>
#include <concepts>
#include <limits>
#include <cmath>

#include "util.hpp"

template <std::floating_point T = double>
class Vector3
{
public:
    constexpr Vector3() : m_e{0.0, 0.0, 0.0} {}
    constexpr Vector3(T e0, T e1, T e2) : m_e{e0, e1, e2} {}

    constexpr T x() const { return m_e[0]; }
    constexpr T y() const { return m_e[1]; }
    constexpr T z() const { return m_e[2]; }

    Vector3 operator-() const { return Vector3(-m_e[0], -m_e[1], -m_e[2]); }

    constexpr T operator[](int i) const { return m_e[i]; }
    constexpr T &operator[](int i) { return m_e[i]; }

    Vector3 &operator+=(const Vector3 &v)
    {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];
        return *this;
    }

    Vector3 &operator*=(T t)
    {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;
        return *this;
    }

    Vector3 &operator/=(T t)
    {
        m_e[0] /= t;
        m_e[1] /= t;
        m_e[2] /= t;
        return *this;
    }

    constexpr T norm() const
    {
        return std::sqrt(squaredNorm());
    }

    constexpr T squaredNorm() const
    {
        return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
    }

    constexpr T length() const
    {
        return norm();
    }

    constexpr T squaredLength() const
    {
        return squaredNorm();
    }

    static constexpr Vector3 random()
    {
        return Vector3(Util::random<T>(), Util::random<T>(), Util::random<T>());
    }

    static constexpr Vector3 random(T min, T max)
    {
        return Vector3(Util::random<T>(min, max), Util::random<T>(min, max), Util::random<T>(min, max));
    }

    constexpr bool nearZero() const
    {
        // Return true if the vector is close to zero in all dimensions
        constexpr T eps = static_cast<T>(1e-8);
        return (std::fabs(m_e[0]) < eps) && (std::fabs(m_e[1]) < eps) && (std::fabs(m_e[2]) < eps);
    }

private:
    std::array<T, 3> m_e;
};

// Point3 is just an alias for Vector3, but useful for code clarity
template <std::floating_point T = double>
using Point3 = Vector3<T>;

using Vector3d = Vector3<double>;
using Point3d = Point3<double>;

using Vector3f = Vector3<float>;
using Point3f = Point3<float>;

// Utility Functions

template <std::floating_point T>
inline std::ostream &operator<<(std::ostream &out, const Vector3<T> &v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

template <std::floating_point T>
inline constexpr Vector3<T> operator+(const Vector3<T> &u, const Vector3<T> &v)
{
    return Vector3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <std::floating_point T>
inline constexpr Vector3<T> operator-(const Vector3<T> &u, const Vector3<T> &v)
{
    return Vector3<T>(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

template <std::floating_point T>
inline constexpr Vector3<T> operator*(const Vector3<T> &u, const Vector3<T> &v)
{
    return Vector3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

template <std::floating_point T>
inline constexpr Vector3<T> operator*(double t, const Vector3<T> &v)
{
    return Vector3<T>(t * v.x(), t * v.y(), t * v.z());
}

template <std::floating_point T>
inline constexpr Vector3<T> operator*(const Vector3<T> &v, double t)
{
    return t * v;
}

template <std::floating_point T>
inline constexpr Vector3<T> operator/(const Vector3<T> &v, double t)
{
    return Vector3<T>(v.x() / t, v.y() / t, v.z() / t);
}

template <std::floating_point T>
inline constexpr Vector3<T> operator/(double t, const Vector3<T> &v)
{
    return Vector3<T>(t / v.x(), t / v.y(), t / v.z());
}

template <std::floating_point T>
inline constexpr T dot(const Vector3<T> &u, const Vector3<T> &v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <std::floating_point T>
inline constexpr Vector3<T> cross(const Vector3<T> &u, const Vector3<T> &v)
{
    return Vector3<T>(u.y() * v.z() - u.z() * v.y(),
                      u.z() * v.x() - u.x() * v.z(),
                      u.x() * v.y() - u.y() * v.x());
}

template <std::floating_point T>
inline constexpr Vector3<T> unitVector(const Vector3<T> &v)
{
    return v / v.norm();
}

template <std::floating_point T>
inline Vector3<T> randomUnitVector()
{
    while (true)
    {
        const auto p = Vector3<T>::random(-1.0, 1.0);
        const T norm = p.squaredNorm();

        // Use constexpr lambda trick to evaluate threshold at compile-time
        // Threshold depends on the precision of T
        // Threshold is 1e-160 for double or more precision and 1e-30 for single precision floats
        constexpr T threshold = []
        {
            if constexpr (std::numeric_limits<T>::digits >= std::numeric_limits<double>::digits)
            {
                return static_cast<T>(1e-160);
            }
            else
            {
                return static_cast<T>(1e-30);
            }
        }();

        if (threshold < norm && norm <= 1.0)
        {
            return p / std::sqrt(norm);
        }
    }
}

template <std::floating_point T>
inline Vector3<T> randomInUnitDisk()
{
    while (true)
    {
        const auto p = Vector3<T>(Util::random<T>(-1, 1), Util::random<T>(-1, 1), 0);
        if (p.squaredNorm() < 1)
        {
            return p;
        }
    }
}

template <std::floating_point T>
inline Vector3<T> randomUnitVectorOnHemisphere(const Vector3<T> &normal)
{
    const Vector3<T> unit_vector = randomUnitVector<T>();
    if (dot(unit_vector, normal) > 0.0) // In the same hemisphere as the normal
        return unit_vector;
    else
        return -unit_vector;
}

template <std::floating_point T>
inline Vector3<T> reflect(const Vector3<T> &v, const Vector3<T> &n)
{
    return v - 2 * dot(v, n) * n;
}

template <std::floating_point T>
inline Vector3<T> refract(const Vector3<T> &v, const Vector3<T> &n, T etaIOverEtaT)
{
    const T cosTheta = std::fmin(dot(-v, n), static_cast<T>(1.0));
    const Vector3<T> rOutPerp = etaIOverEtaT * (v + cosTheta * n);
    const Vector3<T> rOutParallel = -std::sqrt(std::abs(static_cast<T>(1.0) - rOutPerp.squaredNorm())) * n;
    return rOutPerp + rOutParallel;
}

#endif /* INONEWEEKEND_INCLUDE_VECTOR3_HPP */
