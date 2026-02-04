#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <functional>
#include "model.h"

typedef void* (*LoaderFunc)(std::string path);

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();
	void AddLoaderFunc(std::string extension, LoaderFunc loader_func);
	template<typename T>
	T* Load(std::string filePath);
private:
	std::map<std::string, LoaderFunc> extension_to_resource_loader;
	std::map<std::string, void*> file_path_to_resource;
};

void* LoadTexture(std::string path) {
	return Texture::Load(path.c_str());
}

void* LoadModel(std::string path) {
	return new Model(path.c_str());
}

ResourceLoader::ResourceLoader()
{
	AddLoaderFunc(".png", LoadTexture);
	AddLoaderFunc(".glb", LoadModel);
}

ResourceLoader::~ResourceLoader()
{

}

void ResourceLoader::AddLoaderFunc(std::string extension, LoaderFunc loader_func) {
	extension_to_resource_loader[extension] = loader_func;
}

template<typename T>
T* ResourceLoader::Load(std::string filePath) {
	void* ptr = 0;

	// TODO: Check if the filePath can be a different object and be diff key.
	if (file_path_to_resource.contains(filePath)) {
		return (T*)file_path_to_resource[filePath];
	}

	auto i = filePath.find_last_of(".");

	if (i == std::string::npos) {
		throw nullptr;
	}

	auto extension = filePath.substr(i);

	ptr = extension_to_resource_loader[extension](filePath);
	file_path_to_resource[filePath] = ptr;
	//std::printf("Ptr: %d\n", ptr);

	return (T*)ptr;
}


