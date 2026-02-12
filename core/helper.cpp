#include "helper.h"


std::string get_real_file_path(const std::string& path) {
#ifdef _DEBUG
	return DEBUG_ASSETS_PATH + path;
#else
	return COMPILE_ASSETS_PATH + path;
#endif // _DEBUG
}