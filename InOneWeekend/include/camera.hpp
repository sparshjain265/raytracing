/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_CAMERA_HPP
#define INONEWEEKEND_INCLUDE_CAMERA_HPP

#include <concepts>

#include "hittable.hpp"
#include "color.hpp"
#include "ray.hpp"

template <std::floating_point T = double>
class Camera
{
public:
    constexpr Camera() = default;

    constexpr T aspectRatio() const { return m_aspectRatio; }
    constexpr int imageWidth() const { return m_imageWidth; }
    constexpr int numSamplesPerPixel() const { return m_numSamplesPerPixel; }

    void setAspectRatio(T aspectRatio)
    {
        m_aspectRatio = aspectRatio;
    }

    void setImageWidth(int imageWidth)
    {
        m_imageWidth = imageWidth;
    }

    void setNumSamplesPerPixel(int numSamplesPerPixel)
    {
        m_numSamplesPerPixel = numSamplesPerPixel;
    }

    void render(const Hittable<T> &world)
    {
        // Always initialize before rendering
        initialize();

        std::cout << "P3\n"
                  << m_imageWidth << ' ' << m_imageHeight << "\n255\n";

        for (int i = 0; i < m_imageHeight; ++i)
        {
            std::clog << "\rScanlines remaining: " << (m_imageHeight - i) << ' ' << std::flush;
            for (int j = 0; j < m_imageWidth; ++j)
            {
                Color<T> pixelColor(0.0, 0.0, 0.0);
                for (int s = 0; s < m_numSamplesPerPixel; ++s)
                {
                    const auto ray = getRay(i, j);
                    pixelColor += rayColor(ray, world);
                }
                pixelColor *= m_pixelSampleScale;

                writeColor(std::cout, pixelColor);
            }
        }

        std::clog << "\rDone.                                              \n";
    }

private:
    // Publically Accessible Camera Parameters
    T m_aspectRatio{1.0};
    int m_imageWidth{100};
    int m_numSamplesPerPixel{10};

    // Internally Used Camera Parameters
    int m_imageHeight{100};
    Point3<T> m_center{};
    Point3<T> m_pixel00Center{};
    Vector3<T> m_pixelDeltaHorizontal{};
    Vector3<T> m_pixelDeltaVertical{};
    T m_pixelSampleScale{0.1};

    void initialize()
    {
        m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
        m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

        m_center = Point3<T>(0.0, 0.0, 0.0);

        m_pixelSampleScale = static_cast<T>(1.0) / m_numSamplesPerPixel;

        // Viewport Setup
        const T focalLength = 1.0;
        const T viewportHeight = 2.0;
        const T viewportWidth = viewportHeight * (static_cast<T>(m_imageWidth) / m_imageHeight);

        const auto viewportHorizontal = Vector3<T>(viewportWidth, 0.0, 0.0);
        const auto viewportVertical = Vector3<T>(0.0, -viewportHeight, 0.0);

        m_pixelDeltaHorizontal = viewportHorizontal / m_imageWidth;
        m_pixelDeltaVertical = viewportVertical / m_imageHeight;

        const auto viewportTopLeft = m_center -
                                     Vector3<T>(0.0, 0.0, focalLength) -
                                     (viewportHorizontal / 2) -
                                     (viewportVertical / 2);

        m_pixel00Center = viewportTopLeft + 0.5 * (m_pixelDeltaHorizontal + m_pixelDeltaVertical);
    }

    Ray<T> getRay(int i, int j) const
    {
        // Construct a camera ray originating from the origin and directed at a
        // randomly sampled point around the pixel (i, j).

        const auto offset = sampleSquare();
        const auto pixelSample = m_pixel00Center +
                                 ((i + offset.y()) * m_pixelDeltaVertical) +
                                 ((j + offset.x()) * m_pixelDeltaHorizontal);

        const auto rayOrigin = m_center;
        const auto rayDirection = pixelSample - rayOrigin;

        return Ray<T>(rayOrigin, rayDirection);
    }

    Vector3<T> sampleSquare() const
    {
        // Returns the vector to a random point in the [-0.5, -0.5] x [+0.5, +0.5] square
        return Vector3<T>(random<T>() - 0.5, random<T>() - 0.5, 0);
    }

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
};

#endif /* INONEWEEKEND_INCLUDE_CAMERA_HPP */
