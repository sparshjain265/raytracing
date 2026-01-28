/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_HITTABLE_HPP
#define INONEWEEKEND_INCLUDE_HITTABLE_HPP

#include <concepts>
#include <memory>

#include "ray.hpp"
#include "interval.hpp"
#include "material_forward_decl.hpp"
#include "vector3.hpp"

template <std::floating_point T = double>
class HitRecord
{
public:
    constexpr HitRecord() : HitRecord(Point3<T>(), Vector3<T>(), nullptr, 0.0, true) {}

    constexpr HitRecord(
        const Point3<T> &point,
        const Vector3<T> &normal,
        std::shared_ptr<Material<T>> material,
        T t,
        bool frontFace)
        : m_point(point), m_normal(normal), m_material(material), m_t(t), m_frontFace(frontFace)
    {
    }

    constexpr const Point3<T> &point() const { return m_point; }
    constexpr const Vector3<T> &normal() const { return m_normal; }
    constexpr std::shared_ptr<Material<T>> material() const { return m_material; }
    constexpr T t() const { return m_t; }
    constexpr bool frontFace() const { return m_frontFace; }

    void setPoint(const Point3<T> &p) { m_point = p; }
    void setNormal(const Ray<T> &r, const Vector3<T> &outwardNormal)
    {
        // outwardNormal is assumed to be normalized, i.e., unit vector
        m_frontFace = dot(r.direction(), outwardNormal) < 0;
        m_normal = m_frontFace ? outwardNormal : -outwardNormal;
    }
    void setMaterial(std::shared_ptr<Material<T>> material) { m_material = material; }
    void setT(T t) { m_t = t; }

private:
    Point3<T> m_point;
    Vector3<T> m_normal;
    std::shared_ptr<Material<T>> m_material;
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
