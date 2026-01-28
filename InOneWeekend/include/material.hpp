/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_MATERIAL_HPP
#define INONEWEEKEND_INCLUDE_MATERIAL_HPP

#include <concepts>

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
    constexpr Metal(const Color<T> &albedo) : m_albedo(albedo) {}

    virtual ~Metal() override = default;

    virtual bool scatter(
        const Ray<T> &rIn,
        const HitRecord<T> &record,
        Color<T> &attenuation,
        Ray<T> &scattered) const override
    {
        const auto reflected = reflect(unitVector(rIn.direction()), record.normal());
        scattered = Ray<T>(record.point(), reflected);
        attenuation = m_albedo;
        return true;
    }

private:
    Color<T> m_albedo;
};

#endif /* INONEWEEKEND_INCLUDE_MATERIAL_HPP */
