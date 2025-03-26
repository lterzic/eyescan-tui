#include "parser.h"
#include <string>
#include <sstream>

template <typename num_type>
inline std::vector<num_type> read_number_list(std::istream& input)
{
    std::vector<num_type> vector;
    num_type num;
    char tmp;

    while (input >> num) {
        vector.push_back(num);
        input >> tmp;
    }
    return vector;
}

sweep_s read_scan_file(std::istream& input)
{
    sweep_s sweep;
    std::string tmp;
    
    // Skip first 10 lines
    for (int i = 0; i < 10; i++) std::getline(input, tmp);

    // Can be "1d" or "2d"
    std::string scan_type;
    input >> scan_type;
    
    // Read until the first comma (horizontal index) and then
    // store from first index to end of the line in tmp
    std::getline(input, tmp, ',');
    std::getline(input, tmp);

    std::istringstream iss(tmp);
    auto hor_indices = read_number_list<int>(iss);

    while (std::getline(input, tmp)) {
        if (tmp == "Scan End")
            break;
        
        // Convert the line into a stream and read all the numbers
        std::istringstream line_stream(tmp);
        auto line_data = read_number_list<float>(line_stream);
        
        // First number is the line index
        line_data.erase(line_data.begin());

        sweep.ber.push_back(line_data);
    }
    
    sweep.width = hor_indices.size();
    sweep.height = sweep.ber.size();

    return sweep;
}