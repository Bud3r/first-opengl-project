#pragma once

#include <string>

#define FILE_PATH(file) "F:\\Misc\\cmake-test-project\\" #file

std::string get_real_file_path(const std::string& path);