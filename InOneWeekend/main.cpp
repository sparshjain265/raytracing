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

template <std::floating_point T = double>
constexpr Color<T> rayColor(const Ray<T> &r, const Hittable<T> &world)
{
    HitRecord<T> record;
    if (world.hit(r, Interval<T>(static_cast<T>(0), infinity<T>), record))
    {
        const auto N = record.normal();
        constexpr auto ones = Color<T>(1.0, 1.0, 1.0);
        return static_cast<T>(0.5) * (ones + N);
    }

    const auto unitDirection = unitVector(r.direction());
    const T t = static_cast<T>(0.5) * (unitDirection.y() + static_cast<T>(1.0));

    constexpr auto white = Color<T>(1.0, 1.0, 1.0);
    constexpr auto blue = Color<T>(0.5, 0.7, 1.0);

    return (1 - t) * white + t * blue;
}

int main()
{
    // Image Setup
    constexpr auto idealAspectRatio = 16.0 / 9.0;
    constexpr int imageWidth = 400;
    constexpr int idealImageHeight = static_cast<int>(imageWidth / idealAspectRatio);
    constexpr int imageHeight = (idealImageHeight < 1) ? 1 : idealImageHeight;
    constexpr auto aspectRatio = static_cast<double>(imageWidth) / imageHeight;

    // Camera Setup
    constexpr auto focalLength = 1.0;
    constexpr auto cameraCenter = Point3(0.0, 0.0, 0.0);
    constexpr auto viewportHeight = 2.0;
    constexpr auto viewportWidth = viewportHeight * aspectRatio;

    // Viewport Vectors
    constexpr auto viewportHorizontal = Vector3(viewportWidth, 0.0, 0.0);
    constexpr auto viewportVertical = Vector3(0.0, -viewportHeight, 0.0);

    // Pixel Delta Vectors
    constexpr auto pixelDeltaHorizontal = viewportHorizontal / imageWidth;
    constexpr auto pixelDeltaVertical = viewportVertical / imageHeight;

    // Viewport Top-Left Corner
    constexpr auto viewportTopLeft = cameraCenter -
                                     Vector3(0.0, 0.0, focalLength) -
                                     (viewportHorizontal / 2.0) -
                                     (viewportVertical / 2.0);

    // Pixel 00 Center
    constexpr auto pixel00Center = viewportTopLeft +
                                   0.5 * (pixelDeltaHorizontal + pixelDeltaVertical);

    // World Setup
    HittableList<double> world;

    world.add(std::make_shared<Sphere<double>>(Point3(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<Sphere<double>>(Point3(0.0, -100.5, -1.0), 100.0));

    // Render
    std::cout << "P3\n"
              << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int i = 0; i < imageHeight; ++i)
    {
        std::clog << "\rScanlines remaining: " << imageHeight - i << ' ' << std::flush;
        for (int j = 0; j < imageWidth; ++j)
        {
            auto pixelCenter = pixel00Center +
                               i * pixelDeltaVertical +
                               j * pixelDeltaHorizontal;

            auto rayDirection = pixelCenter - cameraCenter;
            Ray ray(cameraCenter, rayDirection);

            auto pixelColor = rayColor(ray, world);
            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                                              \n";

    return 0;
}
