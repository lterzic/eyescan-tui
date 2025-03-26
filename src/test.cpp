#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/filesystem.hpp>
#include "display.h"
#include "test.h"

namespace fs = boost::filesystem;  // Correct the namespace to use Boost.Filesystem
EyeData get_eye(const std::vector<std::vector<std::string>>& scan_list, bool gt) {
    EyeData result;
    result.bathtub = false;
    result.header = "N/A";
    result.date = "N/A";
    result.time = "N/A";
    bool eyedata = false;
    std::vector<std::string> xticks_temp;
    
    for (const auto& row : scan_list) {
        // Date and Time Started
        if (row[0].find("Date and Time Started") == 0) {
            std::string dt = row[1];
            result.date = dt.substr(0, dt.find(' '));
            result.time = dt.substr(dt.find(' ') + 1);
        }

        // Scan Name
        if (row[0].find("Scan Name") == 0) {
            result.header = row[1];
        }

        // Scan End
        if (row[0].find("Scan End") == 0) {
            eyedata = false;
        }

        // Eye data processing
        if (eyedata) {
            result.yticks.push_back(std::stoi(row[0]));
            std::vector<float> img_row;
            for (size_t i = 1; i < row.size(); ++i) {
                img_row.push_back(std::stof(row[i]));
            }
            result.img.push_back(img_row);
        }

        // 2d Statistical or 1d Bathtub
        if (row[0].find("2d statistical") == 0 || row[0].find("1d bathtub") == 0) {
            xticks_temp.clear();
            for (size_t i = 1; i < row.size(); ++i) {
                xticks_temp.push_back(row[i]);
            }
            eyedata = true;
        }
    }

    if (gt) {
        for (auto& elm : result.img) {
            if (elm[0] < elm[1]) {
                for (size_t i = 0; i < elm.size(); ++i) {
                    if (i == 1) {
                        elm[1] = elm[0];
                    } else if (i % 2 == 1) {
                        elm[i] = elm[i - 1];
                    }
                }
            }
            if (elm[0] > elm[1]) {
                for (size_t i = 0; i < elm.size(); ++i) {
                    if (i != 1 && i != elm.size() - 1 && i % 2 == 0) {
                        elm[i] = elm[i + 1];
                    } else if (i == elm.size() - 1) {
                        elm[i] = elm[i - 1];
                    }
                }
            }
        }
    }

    // Convert xticks and yticks to integers
    result.xticks.clear();
    for (const auto& x : xticks_temp) {
        result.xticks.push_back(std::stoi(x));
    }

    // Handle single yticks case (bathtub)
    if (result.yticks.size() == 1) {
        result.bathtub = true;
        std::vector<float> single_row = result.img[0];
        result.img.clear();
        result.img.push_back(single_row);
    }

    return result;
}

// Function to read CSV file and return the data as a vector of vectors of strings
std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> scan_list;
    std::ifstream file(filename, std::ios::binary); // Open in binary mode
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        
        // Read the cells in the row separated by commas
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        scan_list.push_back(row);
    }

    file.close();
    return scan_list;
}

std::string getFirstFileInDirectory(const std::string& folderPath) {
    fs::directory_iterator end_itr; // Used to detect the end of the iteration

    for (fs::directory_iterator itr(folderPath); itr != end_itr; ++itr) {
        if (fs::is_regular_file(itr->status())) {
            return itr->path().string(); // Return the first file found
        }
    }
    return ""; // No file found
}