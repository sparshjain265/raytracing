/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_HITTABLE_HPP
#define INONEWEEKEND_INCLUDE_HITTABLE_HPP

#include <concepts>
#include "ray.hpp"
#include "interval.hpp"

template <std::floating_point T = double>
class HitRecord
{
public:
    constexpr HitRecord() : HitRecord(Point3<T>(), Vector3<T>(), 0.0, true) {}

    constexpr HitRecord(
        const Point3<T> &point,
        const Vector3<T> &normal,
        T t,
        bool frontFace)
        : m_point(point), m_normal(normal), m_t(t), m_frontFace(frontFace)
    {
    }

    constexpr const Point3<T> &point() const { return m_point; }
    constexpr const Vector3<T> &normal() const { return m_normal; }
    constexpr T t() const { return m_t; }
    constexpr bool frontFace() const { return m_frontFace; }

    void setPoint(const Point3<T> &p) { m_point = p; }
    void setT(T t) { m_t = t; }
    void setNormal(const Ray<T> &r, const Vector3<T> &outwardNormal)
    {
        // outwardNormal is assumed to be normalized, i.e., unit vector
        m_frontFace = dot(r.direction(), outwardNormal) < 0;
        m_normal = m_frontFace ? outwardNormal : -outwardNormal;
    }

private:
    Point3<T> m_point;
    Vector3<T> m_normal;
    T m_t;
    bool m_frontFace;
};

template <std::floating_point T = double>
class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(
        const Ray<T> &r,
        Interval<T> rayT,
        HitRecord<T> &record) const = 0;

private:
};

#endif /* INONEWEEKEND_INCLUDE_HITTABLE_HPP */
