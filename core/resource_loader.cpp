#include "resource_loader.h"


void* _load_texture(std::string path) {
	return Texture::Load(path.c_str());
}

void* LoadModel(std::string path) {
	return new Model(path.c_str());
}

ResourceLoader::ResourceLoader()
{
	AddLoaderFunc(".png", _load_texture);
	AddLoaderFunc(".glb", LoadModel);
}

void ResourceLoader::AddLoaderFunc(std::string extension, LoaderFunc loader_func) {
	extension_to_resource_loader_[extension] = loader_func;
}