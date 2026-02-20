#include "texture_loader.h"
#include "../texture.h"


void* TextureLoader::LoadFromFile(std::string file_path) {
	return Texture::FromFile(file_path.c_str());
}

void* TextureLoader::LoadFromMemory(void* ptr, uintmax_t len) {
	return Texture::FromMemory(reinterpret_cast<unsigned char*>(ptr), static_cast<int>(len));
}

std::vector<std::string> TextureLoader::GetSupportedExtensions() {
	return { ".png", ".jpeg" };
}
