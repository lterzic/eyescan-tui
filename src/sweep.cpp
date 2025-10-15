#include "sweep.h"
#include <cmath>
#include <string>
#include <sstream>

namespace eyescan {

template <typename num_type>
static std::vector<num_type> read_number_list(std::istream& input)
{
    std::vector<num_type> vector;
    num_type num;
    char separator;

    while (input >> num) {
        vector.push_back(num);
        input >> separator;
    }
    return vector;
}

sweep::sweep(std::istream& input)
{
    std::string tmp;
    
    // Skip first 10 lines
    for (int i = 0; i < 10; i++) std::getline(input, tmp);

    // Read the scan type from the beginning of the line (can be 1d or 2d)
    // and then read (skip) the rest of the line as it's not needed
    std::string scan_type;
    input >> scan_type;
    std::getline(input, tmp);

    // Parse each line into a row of the sweep
    while (std::getline(input, tmp)) {
        if (tmp == "Scan End")
            break;
        
        // Convert the line into a stream and read all the numbers
        std::istringstream line_stream(tmp);
        auto line_data = read_number_list<float>(line_stream);
        
        // First number is the line index
        line_data.erase(line_data.begin());

        m_data.push_back(line_data);
    }
    
    m_width = m_data[0].size();
    m_height = m_data.size();

    // Make sure that all rows are the same size
    for (const auto& row : m_data)
        assert(row.size() == m_width);
}

sweep sweep::scale(size_t new_width, size_t new_height) const
{
    // Only downscaling allowed
    assert(new_width <= m_width);
    assert(new_height <= m_height);

    // Create a sweep with the new dimensions
    sweep scaled;
    scaled.m_width = new_width;
    scaled.m_height = new_height;
    scaled.m_data = data_t(new_height, row_t(new_width, 0));

    // Multiply a new position with these coefficients
    // to get the position in the original sweep
    float hor_scale = new_width / (float)m_width;
    float ver_scale = new_height / (float)m_height;

    for (size_t r = 0; r < m_height; r++) {
        for (size_t c = 0; c < m_width; c++) {
            size_t scaled_r = std::floor(r * ver_scale);
            size_t scaled_c = std::floor(c * hor_scale);
            scaled.m_data[scaled_r][scaled_c] += m_data[r][c];
        }
    }
    for (auto& row : scaled.m_data) {
        for (auto& col : row) {
            col *= hor_scale * ver_scale;
        }
    }
    return scaled;
}

}