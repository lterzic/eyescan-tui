#pragma once

#include <assert.h>
#include <cstddef>
#include <iostream>
#include <vector>

namespace iscan {

// Single data point of the sweep
using sweep_sample_t = float;

// 2d-grid of samples
class sweep {
public:
    // Create the sweep from an input stream (file or std::cin)
    sweep(std::istream& input);

    // Number of horizontal samples
    size_t get_width() const
    {
        return m_width;
    }

    // Number of vertical samples
    size_t get_height() const
    {
        return m_height;
    }

    // Get the sample at a specific position
    const sweep_sample_t& get_sample(size_t hor, size_t ver) const
    {
        assert(hor < get_width());
        assert(ver < get_height());
        return m_data[ver][hor];
    }

    // Scale this sweep to a different size
    sweep scale(size_t width, size_t height) const;

private:
    size_t m_width;
    size_t m_height;
    std::vector<std::vector<sweep_sample_t>> m_data;
};

}