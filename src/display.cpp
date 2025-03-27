#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/node.hpp>

#include "display.h"

#define CANVAS_HORIZONTAL_SCALE (0.5f)
#define CANVAS_VERTICAL_SCALE (1.f / 3.7f)

/**
 * Draw a canvas to the terminal without scrolling
 */
static void draw_canvas(const ftxui::Canvas& canvas)
{
    // Print without scrolling the terminal
    static std::string reset_position;

    static ftxui::Screen screen = ftxui::Screen(
        canvas.width() * CANVAS_HORIZONTAL_SCALE,
        canvas.height() * CANVAS_VERTICAL_SCALE
    );
    
    auto element = ftxui::canvas(&canvas) | ftxui::border;
    ftxui::Render(screen, element);

    std::cout << reset_position;
    screen.Print();
    std::cout << std::endl;
    reset_position = screen.ResetPosition();
}

/**
 * Create a terminal canvas with the sweep plotted
 */
void display_sweep(const sweep_s& sweep, const colormap_s& colormap)
{
    auto canvas = ftxui::Canvas(sweep.width * 2, sweep.height);

    for (int y = 0; y < sweep.height; y += 4) {
        for (int x = 0; x < sweep.width; x++) {
            float value = -std::log10(sweep.ber[y][x & ~1]);
            float clamped = std::clamp(value, colormap.min, colormap.max);
            
            float ratio = (clamped - colormap.min) / (colormap.max - colormap.min);
            uint8_t hue = (uint8_t)((float)colormap.min_hue + (colormap.max_hue - colormap.min_hue) * ratio);

            canvas.DrawBlock(x * 2, y, true, ftxui::Color::HSV(hue, 255, 255));
            canvas.DrawBlock(x * 2 + 1, y, true, ftxui::Color::HSV(hue, 255, 255));
            canvas.DrawBlock(x * 2, y + 2, true, ftxui::Color::HSV(hue, 255, 255));
            canvas.DrawBlock(x * 2 + 1, y + 2, true, ftxui::Color::HSV(hue, 255, 255));
        }
    }

    draw_canvas(canvas);
}
