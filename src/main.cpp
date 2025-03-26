#include <iostream>
#include <cmath>

#include "types.h"
#include "display.h"
#include "test.h"

ber_diagram_s test_diagram(uint width, uint height)
{
    ber_diagram_s diagram {
        .width = width,
        .height = height,
        .measurements = std::vector<std::vector<point_ber_s>>(
            height,
            std::vector<point_ber_s>(width, {0})
        )
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int xdist = x - (int)width / 2;
            int ydist = y - (int)height / 2;

            diagram.measurements[y][x].sample_count = width * width + height * height;
            diagram.measurements[y][x].error_count = std::pow(xdist*xdist + ydist*ydist, 1.2);
        }
    }

    return diagram;
}

int main() {
    std::string folderPath = "../scans/csv/"; // Change to your folder path

    // Get the first file in the folder
    std::string filePath = getFirstFileInDirectory(folderPath);
    
    if (filePath.empty()) {
        std::cerr << "No files found in the directory!" << std::endl;
    } else {
        std::cout << "Found file: " << filePath << std::endl;
    }
    bool gt = false; // Example, you can change this depending on your logic

    try {
        // Read the CSV file into scan_list
        std::vector<std::vector<std::string>> scan_list = readCSV(filePath);

        // Call get_eye with scan_list and the gt flag
        EyeData result = get_eye(scan_list, gt);

        // Print the results
        std::cout << "Header: " << result.header << "\n";
        std::cout << "Date: " << result.date << "\n";
        std::cout << "Time: " << result.time << "\n";
        std::cout << "Bathtub: " << (result.bathtub ? "True" : "False") << "\n";

        std::cout << "X-ticks: ";
        for (const auto& x : result.xticks) {
            std::cout << x << " ";
        }
        std::cout << "\n";

        std::cout << "Y-ticks: ";
        for (const auto& y : result.yticks) {
            std::cout << y << " ";
        }
        std::cout << "\n";

        // std::cout << "Image Data:\n";
        // for (const auto& row : result.img) {
        //     for (const auto& val : row) {
        //         std::cout << val << " ";
        //     }
        //     std::cout << "\n";
        // }
        
        // Create new struct
        ber_diagram_s_pre diagram;
        diagram.height = result.yticks.size();
        diagram.width = result.xticks.size();
        diagram.measurements = result.img;
        draw_ber_diagram_pre(diagram);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    // draw_ber_diagram(diagram);  // Vizualizuj podatke

    return 0;
}
