#include "resource_loader.h"


void* _load_texture(std::string path) {
	return Texture::Load(path.c_str());
}

void* _load_model(std::string path) {
	return new Model(path.c_str());
}

ResourceLoader::ResourceLoader()
{
	add_loader_func(".png", _load_texture);
	add_loader_func(".glb", _load_model);
}

void ResourceLoader::add_loader_func(std::string extension, LoaderFunc loader_func) {
	extension_to_resource_loader[extension] = loader_func;
}