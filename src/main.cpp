#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <utility>
#include <cmath>

#include "args.h"
#include "display.h"
#include "sweep.h"

// Map (slr, site) pair to a map of channel id to file path. This way
// a list of scans (channels) for a site can be looped through.
static std::map<std::pair<int, int>, std::map<int, std::filesystem::path>> files;

static void parse_file(std::filesystem::path file_path)
{
    // Parse the file name to extract slr, site and channel
    std::stringstream ss(file_path.filename().string());
    std::string token;
    std::getline(ss, token, '_');

    int slr, site, channel;
    ss >> slr;
    ss.get();
    ss >> site;
    ss.get();
    ss >> channel;

    files[{slr, site}][channel] = file_path;
}

static void parse_dir(std::filesystem::path dir_path)
{
    if (!std::filesystem::exists(dir_path)) {
        std::cerr << "Path does not exist!" << std::endl;
        throw "Error";
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
        if (entry.is_regular_file()) {
            parse_file(entry.path());
        }
    }
}

int main(int argc, char* argv[])
{
    auto arg_map = eyescan::parse_args(argc, argv);

    if (arg_map.find("dir") != arg_map.end()) {
        for (auto& dir : arg_map["dir"]) {
            parse_dir(dir);
        }
    }

    if (arg_map.find("files") != arg_map.end()) {
        for (auto& file : arg_map["files"]) {
            parse_file(file);
        }
    }

    int slr = eyescan::get_arg_or_default(arg_map, "slr", 0);
    int site = eyescan::get_arg_or_default(arg_map, "site", 0);
    int cols = eyescan::get_arg_or_default(arg_map, "cols", 2);
    int scan_w = eyescan::get_arg_or_default(arg_map, "scan_w", 32);
    int scan_h = eyescan::get_arg_or_default(arg_map, "scan_h", 16);

    std::cout << "SLR: " << slr << std::endl;
    std::cout << "Site: " << site << std::endl;
    std::cout << "Available scans: " << files[{slr, site}].size() << std::endl;

    eyescan::colormap_s map {.min = 2.5, .max = 4, .min_hue = 180, .max_hue = 0};

    ftxui::Elements rows;
    ftxui::Elements curr_row;
    for (auto& [ch, file] : files[{slr, site}]) {

        std::ifstream in_file;
        in_file.open(file.string());
        eyescan::sweep file_sweep(in_file);

        auto scaled = file_sweep.scale(scan_w, scan_h);
        auto document = eyescan::draw_sweep(scaled, map);
        curr_row.push_back(document);

        if (ch >= cols - 1 && ch % cols == cols - 1) {
            rows.push_back(ftxui::hbox(curr_row));
            curr_row.clear();
        }
    }
    if (!curr_row.empty())
        rows.push_back(ftxui::hbox(curr_row));

    auto grid = ftxui::vbox(rows);

    ftxui::Screen screen(cols * scan_w + 1, rows.size() * scan_h + 1);
    ftxui::Render(screen, grid);
    screen.Print();
    std::cout << "\n";
    return 0;
}
