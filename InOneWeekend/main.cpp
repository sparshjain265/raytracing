/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#include <iostream>
#include <concepts>
#include <memory>

#include "color.hpp"
#include "vector3.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "interval.hpp"
#include "camera.hpp"
#include "material.hpp"

int main()
{
    using T = double;

    // World Setup
    HittableList<T> world;

    const auto groundMaterial = std::make_shared<Lambertial<T>>(Color<T>(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(0, -1000, 0), 1000, groundMaterial));

    for (int i = -11; i < 11; ++i)
    {
        for (int j = -11; j < 11; ++j)
        {
            const auto chooseMaterial = Util::random<T>();
            const Point3<T> center(i + 0.9 * Util::random<T>(), 0.2, j + 0.9 * Util::random<T>());

            if ((center - Point3<T>(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material<T>> sphereMaterial;

                if (chooseMaterial < 0.8)
                {
                    // Diffuse
                    const auto albedo = Color<T>::random() * Color<T>::random();
                    sphereMaterial = std::make_shared<Lambertial<T>>(albedo);
                }
                else if (chooseMaterial < 0.95)
                {
                    // Metal
                    const auto albedo = Color<T>::random(0.5, 1);
                    const auto fuzz = Util::random<T>(0, 0.5);
                    sphereMaterial = std::make_shared<Metal<T>>(albedo, fuzz);
                }
                else
                {
                    // Glass
                    sphereMaterial = std::make_shared<Dielectric<T>>(1.5);
                }

                world.add(std::make_shared<Sphere<T>>(center, 0.2, sphereMaterial));
            }
        }
    }

    world.add(
        std::make_shared<Sphere<T>>(
            Point3<T>(0, 1, 0), 1.0,
            std::make_shared<Dielectric<T>>(1.5)));

    world.add(
        std::make_shared<Sphere<T>>(
            Point3<T>(-4, 1, 0), 1.0,
            std::make_shared<Lambertial<T>>(Color<T>(0.4, 0.2, 0.1))));

    world.add(
        std::make_shared<Sphere<T>>(
            Point3<T>(4, 1, 0), 1.0,
            std::make_shared<Metal<T>>(Color<T>(0.7, 0.6, 0.5), 0.0)));

    Camera<T> camera;
    camera.setAspectRatio(16.0 / 9.0);
    camera.setImageWidth(320); // Increase this for higher resolution image
    camera.setNumSamplesPerPixel(500);
    camera.setMaxReflection(100);

    camera.setVerticalFOV_deg(20);
    camera.setLookFrom(Point3<T>(13, 2, 3));
    camera.setLookAt(Point3<T>(0, 0, 0));
    camera.setVUp(Vector3<T>(0, 1, 0));

    camera.setDefocusAngle_deg(0.6);
    camera.setFocusDist(10);

    camera.render(world);

    return 0;
}
