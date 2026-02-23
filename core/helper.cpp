#include "helper.h"


std::filesystem::path get_real_file_path(const std::filesystem::path& path) {
	return std::filesystem::path(ASSETS_PATH) /= path;
}