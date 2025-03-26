#include <iostream>
#include <cmath>

#include "parser.h"
#include "display.h"

sweep_s test_sweep(uint width, uint height)
{
    sweep_s sweep {
        .width = width,
        .height = height,
        .ber = std::vector<std::vector<float>>(
            height,
            std::vector<float>(width, {0})
        )
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int xdist = x - (int)width / 2;
            int ydist = y - (int)height / 2;
            sweep.ber[y][x] = xdist * xdist + ydist * ydist;
        }
    }
    return sweep;
}

int main()
{
    colormap_s map {.min = 0, .max = 2000, .min_hue = 0, .max_hue = 160};
    display_sweep(test_sweep(120, 80), map);
    return 0;
}
