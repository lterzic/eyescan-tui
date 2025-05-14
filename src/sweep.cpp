#include "sweep.h"
#include <string>
#include <sstream>

namespace iscan {

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

sweep sweep::scale(size_t width, size_t height) const
{
    return *this;
}

}