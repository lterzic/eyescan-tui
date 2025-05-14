#include <iostream>
#include <fstream>
#include <cmath>

#include "sweep.h"
#include "display.h"

int main()
{
    iscan::colormap_s map {.min = 2.5, .max = 4, .min_hue = 180, .max_hue = 0};
    // iscan::sweep cin_sweep(std::cin);
    std::ifstream in_file;
    in_file.open("./../scans/csv/eyescanGTY_0_0_0_2023-Mar-28_13:48:56.csv");
    iscan::sweep file_sweep(in_file);

    std::cout << "Original sweep dimensions (w, h): " << file_sweep.get_width() << " " << file_sweep.get_height() << '\n';
    std::cout << "Terminal dimensions (w, h): " << ftxui::Dimension::Full().dimx << " " << ftxui::Dimension::Full().dimy << '\n';
    
    auto scaled = file_sweep.scale(32, 16);
    auto document = iscan::draw_sweep(scaled, map);
    ftxui::Screen screen(40, 20);
    ftxui::Render(screen, document);

    screen.Print();
    return 0;
}
