/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_CAMERA_HPP
#define INONEWEEKEND_INCLUDE_CAMERA_HPP

#include <chrono>
#include <cmath>
#include <concepts>
#include <iomanip>

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
    constexpr int maxReflection() const { return m_maxReflection; }
    constexpr T verticalFOV() const { return m_verticalFOV; }
    constexpr T verticalFOV_deg() const
    {
        return Util::radiansToDegrees<T>(m_verticalFOV);
    }
    constexpr Point3<T> lookFrom() const { return m_lookFrom; }
    constexpr Point3<T> lookAt() const { return m_lookAt; }
    constexpr Vector3<T> vUp() const { return m_vUp; }
    constexpr T defocusAngle() const { return m_defocusAngle; }
    constexpr T defocusAngle_deg() const
    {
        return Util::radiansToDegrees<T>(m_defocusAngle);
    }
    constexpr T focusDist() const { return m_focusDist; }

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

    void setMaxReflection(int maxReflection)
    {
        // Sets the maximum number of reflections a ray can undergo
        // before being terminated
        // Default is 10
        // Higher values increase realism but also increase render time
        // Very high values can lead to stack overflow due to deep recursion
        // in rayColor()
        m_maxReflection = maxReflection;
    }

    void setVerticalFOV(T verticalFOV)
    {
        m_verticalFOV = verticalFOV;
    }

    void setVerticalFOV_deg(T verticalFOV_deg)
    {
        m_verticalFOV = Util::degreesToRadians<T>(verticalFOV_deg);
    }

    void setLookFrom(const Point3<T> &lookFrom)
    {
        m_lookFrom = lookFrom;
    }

    void setLookAt(const Point3<T> &lookAt)
    {
        m_lookAt = lookAt;
    }

    void setVUp(const Vector3<T> &vUp)
    {
        m_vUp = vUp;
    }

    void setDefocusAngle(T defocusAngle)
    {
        m_defocusAngle = defocusAngle;
    }

    void setDefocusAngle_deg(T defocusAngle_deg)
    {
        m_defocusAngle = Util::degreesToRadians<T>(defocusAngle_deg);
    }

    void setFocusDist(T focusDist)
    {
        m_focusDist = focusDist;
    }

    void render(const Hittable<T> &world)
    {
        // Always initialize before rendering
        initialize();

        std::cout << "P3\n"
                  << m_imageWidth << ' ' << m_imageHeight << "\n255\n";

        const auto startTime = std::chrono::steady_clock::now();

        std::clog << "Rendering..." << std::flush;

        for (int i = 0; i < m_imageHeight; ++i)
        {
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

            // Log progress after each scanline
            const int linesDone = i + 1;
            const auto now = std::chrono::steady_clock::now();
            const auto elapsed = std::chrono::duration<double>(now - startTime).count();
            const double avgTimePerLine = elapsed / linesDone;
            const int linesRemaining = m_imageHeight - linesDone;
            const double etaSeconds = avgTimePerLine * linesRemaining;

            const int etaH = static_cast<int>(etaSeconds) / 3600;
            const int etaM = (static_cast<int>(etaSeconds) % 3600) / 60;
            const int etaS = static_cast<int>(etaSeconds) % 60;

            std::clog << "\rRendering... Progress: " << linesDone << "/" << m_imageHeight
                      << " | ETA: " << std::setfill('0')
                      << std::setw(2) << etaH << ":"
                      << std::setw(2) << etaM << ":"
                      << std::setw(2) << etaS
                      << "    " << std::flush;
        }

        const auto endTime = std::chrono::steady_clock::now();
        const auto totalSeconds = std::chrono::duration<double>(endTime - startTime).count();
        const int totalH = static_cast<int>(totalSeconds) / 3600;
        const int totalM = (static_cast<int>(totalSeconds) % 3600) / 60;
        const int totalS = static_cast<int>(totalSeconds) % 60;

        std::clog << "\rDone. Total time: "
                  << std::setfill('0')
                  << std::setw(2) << totalH << ":"
                  << std::setw(2) << totalM << ":"
                  << std::setw(2) << totalS
                  << "                    \n";
    }

private:
    // Publically Accessible Camera Parameters

    T m_aspectRatio{1.0};         // Ratio of Image Width over Height
    int m_imageWidth{100};        // Image Width in px
    int m_numSamplesPerPixel{10}; // Count of random samples per pixel
    int m_maxReflection{10};      // Maximum number of ray reflections per scene

    T m_verticalFOV{Util::degreesToRadians<T>(90.0)}; // Vertical FoV
    Point3<T> m_lookFrom{0, 0, 0};                    // Point Camera is lookin from
    Point3<T> m_lookAt{0, 0, 0};                      // Point Camera is looking at
    Vector3<T> m_vUp{0, 1, 0};                        // Camera-Relative "up" direction

    T m_defocusAngle{0.0}; // Variation angle of rays through each pixel
    T m_focusDist{0.0};    // Distance from camera lookFrom point to plane of perfect focus

    // Internally Used Camera Parameters

    int m_imageHeight{100};              // Rendered Image Height
    Point3<T> m_center{};                // Camera Center
    Point3<T> m_pixel00Center{};         // Center of Pixel 0, 0
    Vector3<T> m_pixelDeltaHorizontal{}; // Offset of pixel to the right
    Vector3<T> m_pixelDeltaVertical{};   // Offset of pixel below
    T m_pixelSampleScale{0.1};           // Color scale factor for a sum of pixel samples
    Vector3<T> m_u{}, m_v{}, m_w{};      // Camera Frame basis vectors
    Vector3<T> m_defocusDiskU{};         // Defocus disk horizontal radius
    Vector3<T> m_defocusDiskV{};         // Defocus disk vertical radius

    void initialize()
    {
        m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
        m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

        m_center = m_lookFrom;

        m_pixelSampleScale = static_cast<T>(1.0) / m_numSamplesPerPixel;

        // Viewport Setup
        const T halfHeight = std::tan(m_verticalFOV / 2);
        const T viewportHeight = 2 * halfHeight * m_focusDist;
        const T viewportWidth = viewportHeight * (static_cast<T>(m_imageWidth) / m_imageHeight);

        // Calculate the u, v, w unit basis vectors for the camera frame
        m_w = unitVector(m_lookFrom - m_lookAt);
        m_u = unitVector(cross(m_vUp, m_w));
        m_v = cross(m_w, m_u);

        const auto viewportHorizontal = viewportWidth * m_u;
        const auto viewportVertical = viewportHeight * (-m_v);

        m_pixelDeltaHorizontal = viewportHorizontal / m_imageWidth;
        m_pixelDeltaVertical = viewportVertical / m_imageHeight;

        const auto viewportTopLeft = m_center -
                                     (m_focusDist * m_w) -
                                     (viewportHorizontal / 2) -
                                     (viewportVertical / 2);

        m_pixel00Center = viewportTopLeft + 0.5 * (m_pixelDeltaHorizontal + m_pixelDeltaVertical);

        // Calculate the camera defocus disk basis vectors
        const auto defocusRadius = m_focusDist * std::tan(m_defocusAngle / 2);
        m_defocusDiskU = m_u * defocusRadius;
        m_defocusDiskV = m_v * defocusRadius;
    }

    Ray<T> getRay(int i, int j) const
    {
        // Construct a camera ray originating from the origin (defocus disk) and directed at a
        // randomly sampled point around the pixel (i, j).

        const auto offset = sampleSquare();
        const auto pixelSample = m_pixel00Center +
                                 ((i + offset.y()) * m_pixelDeltaVertical) +
                                 ((j + offset.x()) * m_pixelDeltaHorizontal);

        const auto rayOrigin = (m_defocusAngle <= 0) ? m_center : sampleDefocusDisk();
        const auto rayDirection = pixelSample - rayOrigin;

        return Ray<T>(rayOrigin, rayDirection);
    }

    Vector3<T> sampleSquare() const
    {
        // Returns the vector to a random point in the [-0.5, -0.5] x [+0.5, +0.5] square
        return Vector3<T>(Util::random<T>() - 0.5, Util::random<T>() - 0.5, 0);
    }

    Point3<T> sampleDefocusDisk() const
    {
        const auto p = randomInUnitDisk<T>();
        return m_center + (p.x() * m_defocusDiskU) + (p.y() * m_defocusDiskV);
    }

    Color<T> rayColor(const Ray<T> &r, const Hittable<T> &world, int reflectionCount = 0) const
    {
        constexpr auto black = Color<T>(0.0, 0.0, 0.0);
        if (reflectionCount > m_maxReflection)
        {
            return black;
        }

        HitRecord<T> record;

        constexpr T eps = static_cast<T>(0.001);

        if (world.hit(r, Interval<T>(eps, infinity<T>), record))
        {
            Ray<T> scattered;
            Color<T> attenuation;

            if (record.material() && record.material()->scatter(r, record, attenuation, scattered))
            {
                return attenuation * rayColor(scattered, world, reflectionCount + 1);
            }
            else
            {
                return black;
            }
        }

        const auto unitDirection = unitVector(r.direction());
        const T t = static_cast<T>(0.5) * (unitDirection.y() + static_cast<T>(1.0));

        constexpr auto white = Color<T>(1.0, 1.0, 1.0);
        constexpr auto blue = Color<T>(0.5, 0.7, 1.0);

        return (1 - t) * white + t * blue;
    }
};

#endif /* INONEWEEKEND_INCLUDE_CAMERA_HPP */
