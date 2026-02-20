#pragma once

#include <string>
#include <vector>

class ResourceFormatLoader {
public:
	virtual void* LoadFromFile(std::string file_path) { return nullptr; }
	virtual void* LoadFromMemory(void* ptr, uintmax_t len) { return nullptr; }
	virtual std::vector<std::string> GetSupportedExtensions() { return { }; }
};