#pragma once

#include <assert.h>
#include <cstddef>
#include <iostream>
#include <vector>

namespace eyescan {

// 2d-grid of samples
class sweep {
public:
    // Single data point of the sweep
    using sample_t = float;
private:
    // Single row of the data vector
    using row_t = std::vector<sample_t>;
    // Data storage type
    using data_t = std::vector<row_t>;

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
    const sample_t& get_sample(size_t hor, size_t ver) const
    {
        assert(hor < get_width());
        assert(ver < get_height());
        return m_data[ver][hor];
    }

    // Scale this sweep to a different size
    sweep scale(size_t new_width, size_t new_height) const;

private:
    // Empty constructor used when scaling the sweep
    sweep() {}

private:
    size_t m_width;
    size_t m_height;
    data_t m_data;
};

}