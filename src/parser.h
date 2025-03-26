#pragma once

#include <vector>

using uint = unsigned int;

struct sweep_s {
    // add date, time, other info...
    uint width, height;
    std::vector<std::vector<float>> ber;
};