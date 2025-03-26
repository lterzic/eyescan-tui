#pragma once

#include "types.h"

EyeData get_eye(const std::vector<std::vector<std::string>>& scan_list, bool gt);

std::vector<std::vector<std::string>> readCSV(const std::string& filename);

std::string getFirstFileInDirectory(const std::string& folderPath);