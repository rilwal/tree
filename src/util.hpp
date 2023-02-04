#pragma once

#include <string>
#include <vector>

// Utility function to load a whole file
std::vector<uint8_t> load_file(std::string filename, size_t offset = 0, size_t len = -1);
