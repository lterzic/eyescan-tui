#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <utility>
#include <cmath>

#include "sweep.h"
#include "display.h"

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

static auto parse_args(int argc, char* argv[])
{
    std::map<std::string, std::vector<std::string>> arg_map;

    std::string curr_arg;
    for (int argi = 0; argi < argc; argi++) {
        std::string argstr(argv[argi]);
        
        if (argstr.substr(0, 2) == "--") {
            curr_arg = argstr.substr(2);
            continue;
        } else {
            arg_map[curr_arg].push_back(argstr);
        }
    }

    return arg_map;
}

int main(int argc, char* argv[])
{
    auto arg_map = parse_args(argc, argv);

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

    int slr = arg_map.find("slr") == arg_map.end() ? 0 : std::stoi(arg_map["slr"][0]);
    int site = arg_map.find("site") == arg_map.end() ? 0 : std::stoi(arg_map["site"][0]);
    int cols = arg_map.find("cols") == arg_map.end() ? 2 : std::stoi(arg_map["cols"][0]);
    int scan_w = 32, scan_h = 16; // TODO: As arg

    std::cout << "SLR: " << slr << std::endl;
    std::cout << "Site: " << site << std::endl;
    std::cout << "Available scans: " << files[{slr, site}].size() << std::endl;

    iscan::colormap_s map {.min = 2.5, .max = 4, .min_hue = 180, .max_hue = 0};

    ftxui::Elements rows;
    ftxui::Elements curr_row;
    for (auto& [ch, file] : files[{slr, site}]) {

        std::ifstream in_file;
        in_file.open(file.string());
        iscan::sweep file_sweep(in_file);

        auto scaled = file_sweep.scale(scan_w, scan_h);
        auto document = iscan::draw_sweep(scaled, map);
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
