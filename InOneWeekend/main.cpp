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

    auto groundMaterial = std::make_shared<Lambertial<T>>(Color<T>(0.8, 0.8, 0.0));
    auto centerMaterial = std::make_shared<Lambertial<T>>(Color<T>(0.1, 0.2, 0.5));
    auto leftMaterial = std::make_shared<Dielectric<T>>(1.50);
    auto bubbleMaterial = std::make_shared<Dielectric<T>>(1.00 / 1.50);
    auto rightMaterial = std::make_shared<Metal<T>>(Color<T>(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere<T>>(Point3<T>(0.0, -100.5, -1.0), 100.0, groundMaterial));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(0.0, 0.0, -1.2), 0.5, centerMaterial));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(-1.0, 0.0, -1.0), 0.5, leftMaterial));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(-1.0, 0.0, -1.0), 0.4, bubbleMaterial));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(1.0, 0.0, -1.0), 0.5, rightMaterial));

    Camera<T> camera;
    camera.setAspectRatio(16.0 / 9.0);
    camera.setImageWidth(400);
    camera.setNumSamplesPerPixel(100);
    camera.setMaxReflection(100);
    camera.setVerticalFOV_deg(45);

    camera.render(world);

    return 0;
}
