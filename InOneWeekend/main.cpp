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
constexpr bool hitSphere(
    const Point3<T> &center,
    T radius,
    const Ray<T> &r)
{
    const auto o = center - r.origin();
    const auto a = dot(r.direction(), r.direction());
    const auto b = -2.0 * dot(r.direction(), o);
    const auto c = dot(o, o) - radius * radius;
    const auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

template <std::floating_point T = double>
constexpr Color<T> rayColor(const Ray<T> &r)
{
    if (hitSphere(Point3<T>(0.0, 0.0, -1.0), 0.5, r))
    {
        return Color<T>(1.0, 0.0, 0.0); // Red for sphere hit
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
