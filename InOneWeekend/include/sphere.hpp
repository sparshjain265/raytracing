/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_SPHERE_HPP
#define INONEWEEKEND_INCLUDE_SPHERE_HPP

#include <concepts>
#include <memory>

#include "hittable.hpp"
#include "vector3.hpp"
#include "interval.hpp"
#include "material_forward_decl.hpp"

template <std::floating_point T = double>
class Sphere : public Hittable<T>
{
public:
    constexpr Sphere(const Point3<T> &center, T radius, std::shared_ptr<Material<T>> material)
        : m_center(center), m_radius(radius), m_material(material) {}

    virtual ~Sphere() override = default;

    constexpr const Point3<T> &center() const { return m_center; }
    constexpr T radius() const { return m_radius; }

    virtual bool hit(
        const Ray<T> &r,
        Interval<T> rayT,
        HitRecord<T> &record) const override
    {
        const auto oc = m_center - r.origin();
        const auto a = r.direction().squaredNorm();
        const auto h = dot(r.direction(), oc);
        const auto c = oc.squaredNorm() - m_radius * m_radius;
        const auto discriminant = h * h - a * c;

        if (discriminant < 0)
        {
            return false;
        }

        const auto sqrtD = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtD) / a;
        if (!rayT.surrounds(root))
        {
            root = (h + sqrtD) / a;
            if (!rayT.surrounds(root))
            {
                return false;
            }
        }

        record.setT(root);
        record.setPoint(r.at(root));
        const auto outwardNormal = (record.point() - m_center) / m_radius;
        record.setNormal(r, outwardNormal);
        record.setMaterial(m_material);

        return true;
    }

private:
    Point3<T> m_center;
    T m_radius;
    std::shared_ptr<Material<T>> m_material;
};

#endif /* INONEWEEKEND_INCLUDE_SPHERE_HPP */
