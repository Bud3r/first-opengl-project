#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class ResourceFormatLoader {
public:
	virtual void* LoadFromFile(std::string file_path) { return nullptr; }
	virtual void* LoadFromMemory(void* ptr, uintmax_t len) { return nullptr; }
	virtual std::vector<std::string> GetSupportedExtensions() { return { }; }
	virtual void CreateGeneratedAsset(std::ifstream& base_file, std::ofstream& asset_file, std::filesystem::path base_file_path) {
		asset_file << base_file.rdbuf();
	}
	virtual bool ShouldCreateGeneratedAsset() {
		return false; 
	}
};