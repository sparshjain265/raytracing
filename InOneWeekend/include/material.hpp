/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_MATERIAL_HPP
#define INONEWEEKEND_INCLUDE_MATERIAL_HPP

#include <concepts>
#include <cmath>

#include "hittable.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "material_forward_decl.hpp"

template <std::floating_point T>
class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray<T> &rIn,
        const HitRecord<T> &record,
        Color<T> &attenuation,
        Ray<T> &scattered) const = 0;
};

template <std::floating_point T = double>
class Lambertial : public Material<T>
{
public:
    constexpr Lambertial(const Color<T> &albedo) : m_albedo(albedo) {}

    virtual ~Lambertial() override = default;

    virtual bool scatter(
        [[maybe_unused]] const Ray<T> &rIn,
        const HitRecord<T> &record,
        Color<T> &attenuation,
        Ray<T> &scattered) const override
    {
        auto scatterDirection = record.normal() + randomUnitVector<T>();

        // Catch degenerate scatter direction
        if (scatterDirection.nearZero())
        {
            scatterDirection = record.normal();
        }

        scattered = Ray<T>(record.point(), scatterDirection);
        attenuation = m_albedo;
        return true;
    }

private:
    Color<T> m_albedo;
};

template <std::floating_point T = double>
class Metal : public Material<T>
{
public:
    constexpr Metal(const Color<T> &albedo, T fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual ~Metal() override = default;

    virtual bool scatter(
        const Ray<T> &rIn,
        const HitRecord<T> &record,
        Color<T> &attenuation,
        Ray<T> &scattered) const override
    {
        auto reflected = reflect(unitVector(rIn.direction()), record.normal());
        reflected += unitVector<T>(reflected) + (m_fuzz * randomUnitVector<T>());
        scattered = Ray<T>(record.point(), reflected);
        attenuation = m_albedo;
        return (dot(scattered.direction(), record.normal()) > 0);
    }

private:
    Color<T> m_albedo;
    T m_fuzz;
};

template <std::floating_point T = double>
class Dielectric : public Material<T>
{
public:
    constexpr Dielectric(T refractiveIndex) : m_refractiveIndex(refractiveIndex) {}

    virtual ~Dielectric() override = default;

    virtual bool scatter(
        const Ray<T> &rIn,
        const HitRecord<T> &record,
        Color<T> &attenuation,
        Ray<T> &scattered) const override
    {
        attenuation = Color<T>(1.0, 1.0, 1.0);
        const T etaIOverEtaT = record.frontFace() ? (static_cast<T>(1.0) / m_refractiveIndex) : m_refractiveIndex;

        const auto unitDirection = unitVector(rIn.direction());

        const T cosTheta = std::fmin(dot(-unitDirection, record.normal()), static_cast<T>(1.0));
        const T sinTheta = std::sqrt(static_cast<T>(1.0) - cosTheta * cosTheta);

        bool cannotRefract = (etaIOverEtaT * sinTheta) > static_cast<T>(1.0);
        Vector3<T> direction;

        if (cannotRefract || reflectance(cosTheta, etaIOverEtaT) > Util::random<T>())
        {
            direction = reflect(unitDirection, record.normal());
        }
        else
        {
            direction = refract(unitDirection, record.normal(), etaIOverEtaT);
        }

        scattered = Ray<T>(record.point(), direction);
        return true;
    }

private:
    T m_refractiveIndex;

    static T reflectance(T cosine, T refractionIndex)
    {
        // Use Schlick's approximation for reflectance
        T r0 = (static_cast<T>(1.0) - refractionIndex) / (static_cast<T>(1.0) + refractionIndex);
        r0 = r0 * r0;
        return r0 + (static_cast<T>(1.0) - r0) * std::pow((static_cast<T>(1.0) - cosine), 5);
    }
};

#endif /* INONEWEEKEND_INCLUDE_MATERIAL_HPP */
