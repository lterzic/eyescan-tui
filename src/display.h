#pragma once

#include <cstdint>
#include "parser.h"

/**
 * Clamp diagram values between min and max
 * Scale hue linearly with value from min_hue to max_hue
 * 
 * @note hue = min_hue + (max_hue - min_hue) * (clamped_value - min) / (max - min)
 */
struct colormap_s {
    float min, max;
    uint8_t min_hue, max_hue;
};

/**
 * Draw the sweep in the terminal
 */
void display_sweep(const sweep_s& sweep, const colormap_s& colormap);
