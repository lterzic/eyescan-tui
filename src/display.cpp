#include <iostream>
#include <string>
#include <cmath>

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/node.hpp>

#include "display.h"

static void draw_canvas(const ftxui::Canvas& canvas)
{
    // Print without scrolling the terminal
    static std::string reset_position;

    static ftxui::Screen screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),
        ftxui::Dimension::Full()
    );

    // auto dim = ftxui::Dimensions {
    //     canvas.width() / 2,
    //     canvas.height() / 3
    // };
    // ftxui::Screen screen = ftxui::Screen::Create(dim);
    
    auto element = ftxui::canvas(&canvas);
    ftxui::Render(screen, element);

    std::cout << reset_position;
    screen.Print();
    std::cout << std::endl;
    reset_position = screen.ResetPosition();
}

void draw_ber_diagram(const ber_diagram_s& diagram)
{
    auto canvas = ftxui::Canvas(diagram.width, diagram.height);

    for (int y = 0; y < diagram.height; y++) {
        for (int x = 0; x < diagram.width; x++) {
            auto const& measurement = diagram.measurements[y][x];
            
            float ber = measurement.error_count / (float)measurement.sample_count;
            uint8_t hue = (uint8_t)(-std::log10(ber) * 20.f);

            canvas.DrawPoint(x, y, true, ftxui::Color::HSV(hue, 255, 255));
        }
    }

    draw_canvas(canvas);
}

void draw_ber_diagram_pre(const ber_diagram_s_pre& diagram)
{
    auto canvas = ftxui::Canvas(diagram.width, diagram.height);

    for (int y = 0; y < diagram.height; y++) {
        for (int x = 0; x < diagram.width; x++) {
            auto const& measurement = diagram.measurements[y][x];
            
            uint8_t hue = (uint8_t)(-std::log10(measurement) * 20.f);

            canvas.DrawPoint(x, y, true, ftxui::Color::HSV(hue, 255, 255));
        }
    }

    draw_canvas(canvas);
}
