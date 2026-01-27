/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#include <iostream>
#include "color.hpp"
#include "vector3.hpp"

int main()
{
    // Image

    int width{256};
    int height{256};

    // Render
    std::cout << "P3\n"
              << width << ' ' << height << "\n255\n";

    for (int i = 0; i < height; ++i)
    {
        std::clog << "\rScanlines remaining: " << height - i << ' ' << std::flush;
        for (int j = 0; j < width; ++j)
        {
            auto pixelColor = Color<double>(
                static_cast<double>(j) / (width - 1),
                static_cast<double>(i) / (height - 1),
                0.0);

            writeColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                                              \n";

    return 0;
}
