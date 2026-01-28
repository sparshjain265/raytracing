/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_SPHERE_HPP
#define INONEWEEKEND_INCLUDE_SPHERE_HPP

#include "hittable.hpp"
#include "vector3.hpp"
#include <concepts>

template <std::floating_point T = double>
class Sphere : public Hittable<T>
{
public:
    constexpr Sphere() : Sphere(Point3<T>(), static_cast<T>(1.0)) {}

    constexpr Sphere(const Point3<T> &center, T radius)
        : m_center(center), m_radius(radius) {}

    virtual ~Sphere() = default;

    constexpr const Point3<T> &center() const { return m_center; }
    constexpr T radius() const { return m_radius; }

    virtual bool hit(
        const Ray<T> &r,
        T t_min,
        T t_max,
        HitRecord<T> &record) const override
    {
        const auto oc = m_center - r.origin();
        const auto a = r.direction().squared_norm();
        const auto h = dot(r.direction(), oc);
        const auto c = oc.squared_norm() - m_radius * m_radius;
        const auto discriminant = h * h - a * c;

        if (discriminant < 0)
        {
            return false;
        }

        const auto sqrtD = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtD) / a;
        if (root < t_min || root > t_max)
        {
            root = (h + sqrtD) / a;
            if (root < t_min || root > t_max)
            {
                return false;
            }
        }

        record.setT(root);
        record.setPoint(r.at(root));
        const auto outwardNormal = (record.point() - m_center) / m_radius;
        record.setNormal(r, outwardNormal);

        return true;
    }

private:
    Point3<T> m_center;
    T m_radius;
};

#endif /* INONEWEEKEND_INCLUDE_SPHERE_HPP */
