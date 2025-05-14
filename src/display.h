#pragma once

#include "sweep.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <algorithm>
#include <cstdint>

namespace iscan {

// Clamp diagram values between min and max
// Scale hue linearly with value from min_hue to max_hue
struct colormap_s {
    float min, max;
    uint8_t min_hue, max_hue;

    uint8_t get_hue(float input) const
    {
        float clamped = std::clamp(input, min, max);
        float ratio = (clamped - min) / (max - min);
        return (uint8_t)((float)min_hue + (max_hue - min_hue) * ratio);
    }
};

// Creates an array of colored white-spaces to visualize a sweep
ftxui::Element draw_sweep(const sweep& sweep, const colormap_s& colormap);

}