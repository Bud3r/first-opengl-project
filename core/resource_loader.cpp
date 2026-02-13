#include "resource_loader.h"


namespace {
	void* LoadTexture(std::string path) {
		return Texture::Load(path.c_str());
	}

	void* LoadModel(std::string path) {
		return new Model(path.c_str());
	}
} // namespace

ResourceLoader::ResourceLoader()
{
	AddLoaderFunc(".png", LoadTexture);
	AddLoaderFunc(".glb", LoadModel);
}

void ResourceLoader::AddLoaderFunc(std::string extension, LoaderFunc loader_func) {
	extension_to_resource_loader_[extension] = loader_func;
}