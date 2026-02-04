#pragma once

#include <string>
#include <map>
#include "model.h"
#include "texture.h"

typedef void* (*LoaderFunc)(std::string path);

class ResourceLoader
{
public:
	ResourceLoader();
	void add_loader_func(std::string extension, LoaderFunc loader_func);
	template<typename T>
	T* load(std::string p_file_path);
private:
	std::map<std::string, LoaderFunc> extension_to_resource_loader;
	std::map<std::string, void*> file_path_to_resource;
};

void* _load_texture(std::string path);

void* _load_model(std::string path);

template<typename T>
T* ResourceLoader::load(std::string p_file_path) {
	void* ptr = 0;

	// TODO: Check if the filePath can be a different object and be diff key.
	if (file_path_to_resource.contains(p_file_path)) {
		return (T*)file_path_to_resource[p_file_path];
	}

	auto i = p_file_path.find_last_of(".");

	if (i == std::string::npos) {
		throw nullptr;
	}

	auto extension = p_file_path.substr(i);

	ptr = extension_to_resource_loader[extension](p_file_path);
	file_path_to_resource[p_file_path] = ptr;

	return (T*)ptr;
}
