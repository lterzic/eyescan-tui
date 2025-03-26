#pragma once

#include <iostream>
#include <vector>

using uint = unsigned int;

struct sweep_s {
    // add date, time, other info...
    uint width, height;
    std::vector<std::vector<float>> ber;
};

/**
 * Read a single scan file (1d or 2d)
 */
sweep_s read_scan_file(std::istream& input);