#pragma once

#include <string>
#include <istream>
#include <fstream>
#include <sstream>

#include "texture_loader.h"
#include "../shader_program.h"


class ShaderProgramLoader : public ResourceFormatLoader {
public:
	virtual void* LoadFromFile(std::string file_path) override;
	virtual void* LoadFromMemory(void* ptr, uintmax_t len) override;
	virtual std::vector<std::string> GetSupportedExtensions() override;
	virtual void CreateGeneratedAsset(std::ifstream& base_file, std::ofstream& asset_file, std::filesystem::path base_file_path) override;
	virtual bool ShouldCreateGeneratedAsset() override {
		return true;
	}
private:
	void* LoadFromStream(std::istream& stream, std::filesystem::path file_path = "");
};