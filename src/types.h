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