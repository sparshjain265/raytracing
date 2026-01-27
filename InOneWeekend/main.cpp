/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#include <iostream>
#include <concepts>
#include "color.hpp"
#include "vector3.hpp"
#include "ray.hpp"

template <std::floating_point T = double>
constexpr T hitSphere(
    const Point3<T> &center,
    T radius,
    const Ray<T> &r)
{
    const auto oc = center - r.origin();
    const auto a = dot(r.direction(), r.direction());
    const auto h = dot(r.direction(), oc);
    const auto c = dot(oc, oc) - radius * radius;
    const auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
        return static_cast<T>(-1.0);
    }
    else
    {
        return (h - std::sqrt(discriminant)) / a;
    }
}

template <std::floating_point T = double>
constexpr Color<T> rayColor(const Ray<T> &r)
{
    constexpr auto center = Point3<T>(0.0, 0.0, -1.0);
    constexpr auto radius = static_cast<T>(0.5);
    const auto t = hitSphere(center, radius, r);
    if (t > 0)
    {
        const auto N = unitVector(r.at(t) - center);
        constexpr auto ones = Color<T>(1.0, 1.0, 1.0);
        return static_cast<T>(0.5) * (ones + N);
    }

    const auto unitDirection = unitVector(r.direction());
    const T a = static_cast<T>(0.5) * (unitDirection.y() + static_cast<T>(1.0));

    constexpr auto white = Color<T>(1.0, 1.0, 1.0);
    constexpr auto blue = Color<T>(0.5, 0.7, 1.0);

    return (1 - a) * white + a * blue;
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

    // Pixel 00 Location
    constexpr auto pixel00Location = viewportTopLeft +
                                     0.5 * (pixelDeltaHorizontal + pixelDeltaVertical);

    // Render
    std::cout << "P3\n"
              << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int i = 0; i < imageHeight; ++i)
    {
        std::clog << "\rScanlines remaining: " << imageHeight - i << ' ' << std::flush;
        for (int j = 0; j < imageWidth; ++j)
        {
            auto pixelLocation = pixel00Location +
                                 i * pixelDeltaVertical +
                                 j * pixelDeltaHorizontal;

            auto rayDirection = pixelLocation - cameraCenter;
            auto ray = Ray(cameraCenter, rayDirection);
            auto pixelColor = rayColor(ray);
            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                                              \n";

    return 0;
}
