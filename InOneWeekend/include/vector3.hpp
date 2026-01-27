/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_VEC3_HPP
#define INONEWEEKEND_INCLUDE_VEC3_HPP

#include <cmath>
#include <iostream>
#include <array>
#include <concepts>

template <std::floating_point T = double>
class Vector3
{
public:
    Vector3() : m_e{0.0, 0.0, 0.0} {}
    Vector3(T e0, T e1, T e2) : m_e{e0, e1, e2} {}

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
        return std::sqrt(squared_norm());
    }

    constexpr T squared_norm() const
    {
        return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
    }

    constexpr T length() const
    {
        return norm();
    }

    constexpr T squared_length() const
    {
        return squared_norm();
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
inline constexpr double dot(const Vector3<T> &u, const Vector3<T> &v)
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

#endif /* INONEWEEKEND_INCLUDE_VEC3_HPP */
