#pragma once

#include <string>
#include <map>
#include <filesystem>
#include "helper.h"
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
	std::string real_path = get_real_file_path(p_file_path);

	void* ptr = 0;

	if (!std::filesystem::exists(real_path)) {
		throw std::logic_error(real_path + " does not exist.");
	}

	if (file_path_to_resource.contains(real_path)) {
		std::cout << ("Loaded already loaded file: " + real_path) << std::endl;
		return (T*)file_path_to_resource[real_path];
	}

	auto i = real_path.find_last_of(".");

	if (i == std::string::npos) {
		throw std::logic_error(real_path + " has no file extension");
	}

	auto extension = real_path.substr(i);

	ptr = extension_to_resource_loader[extension](real_path);
	file_path_to_resource[real_path] = ptr;
	

	if (i == std::string::npos) {
		throw std::logic_error("Function failed to load file " + real_path);
	}
	
	std::cout << ("Loaded file: " + real_path) << std::endl;
	return (T*)ptr;
}
