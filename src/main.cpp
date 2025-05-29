#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <utility>
#include <cmath>

#include "sweep.h"
#include "display.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path to scans folder>" << std::endl;
        return 1;
    }

    // First argument is the path to the folder containing all the scans
    std::filesystem::path input_path = argv[1];
    if (!std::filesystem::exists(input_path)) {
        std::cerr << "Path does not exist!" << std::endl;
        return 1;
    }

    // Generate a list of all files in the folder
    std::map<std::tuple<int, int, int>, std::filesystem::path> files;
    // Number of channels for (slr, site)
    std::map<std::pair<int, int>, int> channel_count;

    for (const auto& entry : std::filesystem::directory_iterator(input_path)) {
        if (entry.is_regular_file()) {
            // Parse the file name to extract slr, site and channel
            std::stringstream ss(entry.path().filename().string());
            std::string token;
            std::getline(ss, token, '_');

            int slr, site, channel;
            ss >> slr;
            ss.get();
            ss >> site;
            ss.get();
            ss >> channel;

            files[{slr, site, channel}] = entry.path();
            // Compute the number of channels
            channel_count[{slr, site}] = std::max(channel_count[{slr, site}], channel + 1);
        }
    }

    // Replace with args
    int slr = 0;
    int site = 0;
    int columns = 2;

    iscan::colormap_s map {.min = 2.5, .max = 4, .min_hue = 180, .max_hue = 0};

    for (int ch = 0; ch < channel_count[{slr, site}]; ch++) {
        const auto& file = files[{slr, site, ch}];
        std::cout << file.filename().string() << std::endl;

        std::ifstream in_file;
        in_file.open(file.string());
        iscan::sweep file_sweep(in_file);

        auto scaled = file_sweep.scale(32, 16);
        auto document = iscan::draw_sweep(scaled, map);
        ftxui::Screen screen(40, 20);
        ftxui::Render(screen, document);

        screen.Print();
    }
    return 0;
}
