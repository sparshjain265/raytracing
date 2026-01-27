/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#include <iostream>

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
            auto r = static_cast<double>(j) / (width - 1);
            auto g = static_cast<double>(i) / (height - 1);
            auto b = static_cast<double>(0.0);

            auto ir = static_cast<int>(255.999 * r);
            auto ig = static_cast<int>(255.999 * g);
            auto ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.                                              \n";

    return 0;
}
