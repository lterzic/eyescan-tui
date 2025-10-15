#include "display.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

namespace eyescan {

ftxui::Element draw_sweep(const sweep& sweep, const colormap_s& colormap)
{
    //TODO: Resize sweep to canvas size
    constexpr uint8_t saturation = 255;
    constexpr uint8_t value = 255;
    
    ftxui::Elements rows;
    for (size_t r = 0; r < sweep.get_height(); r++) {
        ftxui::Elements row_samples;
        for (size_t c = 0; c < sweep.get_width(); c++) {
            uint8_t hue = colormap.get_hue(sweep.get_sample(c, r));
            row_samples.push_back(
                ftxui::text(" ") |
                ftxui::bgcolor(ftxui::Color::HSV(hue, saturation, value)));
        }
        rows.push_back(ftxui::hbox(row_samples));
    }
    return ftxui::vbox(rows);
}

}