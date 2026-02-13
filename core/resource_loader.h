#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <memory>
#include "helper.h"
#include "model.h"
#include "texture.h"

typedef void* (*LoaderFunc)(std::string path);

class ResourceLoader
{
public:
	ResourceLoader();
	void AddLoaderFunc(std::string extension, LoaderFunc loader_func);
	template<typename T>
	std::shared_ptr<T> Load(std::string p_file_path);
private:
	std::map<std::string, LoaderFunc> extension_to_resource_loader_;
	std::map<std::string, std::weak_ptr<void>> file_path_to_resource_;
};

template<typename T>
std::shared_ptr<T> ResourceLoader::Load(std::string p_file_path) {
	std::string real_path = get_real_file_path(p_file_path);

	void* ptr = 0;

	if (!std::filesystem::exists(real_path)) {
		throw std::logic_error(real_path + " does not exist.");
	}

	if (file_path_to_resource_.contains(real_path)) {
		std::cout << ("Loaded already loaded file: " + real_path) << std::endl;
		std::weak_ptr<void> weak = file_path_to_resource_[real_path];

		if (!weak.expired()) {
			return std::static_pointer_cast<T>(weak.lock());
		}
	}
	
	int i = real_path.find_last_of(".");

	if (i == std::string::npos) {
		throw std::logic_error(real_path + " has no file extension");
	}

	std::string extension = real_path.substr(i);

	ptr = extension_to_resource_loader_[extension](real_path);

	if (ptr == nullptr) {
		throw std::logic_error("Function failed to load file " + real_path);
	}
	
	std::shared_ptr<T> shared((T*)ptr);
	file_path_to_resource_[real_path] = shared;
	
	std::cout << ("Loaded file: " + real_path) << std::endl;
	return shared;
}
