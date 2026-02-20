#include "model_loader.h"
#include "../model.h"

void* ModelLoader::LoadFromFile(std::string file_path) {
	return new Model(file_path.c_str());
}

void* ModelLoader::LoadFromMemory(void* ptr, uintmax_t len) {
	return new Model(ptr, len);
}

std::vector<std::string> ModelLoader::GetSupportedExtensions() {
	return { ".glb" };
}
