#pragma once

#include <vector>

typedef unsigned int uint;

/**
 * Bit error rate (BER) measurement at a single point
 */
struct point_ber_s {
    uint sample_count;
    uint error_count;
};

/**
 * Grid of BER measurements
 */
struct ber_diagram_s {
    uint width;
    uint height;
    std::vector<std::vector<point_ber_s>> measurements;
};

/**
 * Grid of pre-calculated BER measurements 
 */
struct ber_diagram_s_pre {
    uint width;
    uint height;
    std::vector<std::vector<float>> measurements;
};

struct EyeData {
    std::vector<std::vector<float>> img;
    std::vector<int> xticks;
    std::vector<int> yticks;
    std::string date;
    std::string time;
    std::string header;
    bool bathtub;
};