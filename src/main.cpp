#include <iostream>
#include <cmath>

#include "types.h"
#include "display.h"

ber_diagram_s test_diagram(uint width, uint height)
{
    ber_diagram_s diagram {
        .width = width,
        .height = height,
        .measurements = std::vector<std::vector<point_ber_s>>(
            height,
            std::vector<point_ber_s>(width, {0})
        )
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int xdist = x - (int)width / 2;
            int ydist = y - (int)height / 2;

            diagram.measurements[y][x].sample_count = width * width + height * height;
            diagram.measurements[y][x].error_count = std::pow(xdist*xdist + ydist*ydist, 1.2);
        }
    }

    return diagram;
}

int main()
{
    draw_ber_diagram(test_diagram(120, 80));
    return 0;
}
