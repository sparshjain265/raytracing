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

int main()
{
    using T = double;

    // World Setup
    HittableList<T> world;

    world.add(std::make_shared<Sphere<T>>(Point3<T>(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<Sphere<T>>(Point3<T>(0.0, -100.5, -1.0), 100.0));

    Camera<T> camera;
    camera.setAspectRatio(16.0 / 9.0);
    camera.setImageWidth(400);

    camera.render(world);

    return 0;
}
