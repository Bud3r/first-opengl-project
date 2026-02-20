#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <cassert>
#include <fstream>
#include <iostream>

#include "helper.h"
#include "resource_loaders/resource_format_loader.h"



class ResourceLoader {
public:
	enum class LoadMode {
		Directory,
		AssetPack
	};

	ResourceLoader();
	bool CanLoad(std::filesystem::path path) const;
	bool CanLoadExtension(std::filesystem::path extension) const;
	const std::vector<char> GetAssetData(std::string asset);
	void SaveAssetPack(const std::filesystem::path& assets_directory_path, const std::filesystem::path& save_path);
	void LoadAssetPack(const std::filesystem::path& load_path);
	bool AssetExists(std::string asset);

	template<typename T>
	requires std::is_base_of_v<ResourceFormatLoader, T>
	void AddResourceFormatLoader() {
		std::unique_ptr<T> format_loader = std::make_unique<T>();

		for (std::string extension : format_loader->GetSupportedExtensions()) {
			extension_to_format_loader_[extension] = std::make_unique<T>();
		}
	}

	template<typename T>
	std::shared_ptr<T> Load(std::string p_file_path) {
		std::string real_path = get_real_file_path(p_file_path);

		void* ptr = 0;

		if (file_path_to_resource_.contains(real_path)) {
			std::weak_ptr<void> weak = file_path_to_resource_[real_path];

			if (!weak.expired()) {
				//std::cout << ("Already loaded file: " + real_path) << std::endl;

				return std::static_pointer_cast<T>(weak.lock());
			}
		}

		size_t i = real_path.find_last_of(".");

		if (i == std::string::npos) {
			throw std::logic_error(real_path + " has no file extension");
		}

		std::string extension = real_path.substr(i);

		switch (load_mode)
		{
			case LoadMode::Directory:
			ptr = PtrFromFile(real_path, extension);
			break;
			case LoadMode::AssetPack:
			ptr = PtrFromPack(p_file_path, extension);
			break;
			default:
			break;
		}

		if (ptr == nullptr) {
			throw std::logic_error("Function failed to load file " + real_path);
		}

		std::shared_ptr<T> shared((T*)ptr);
		file_path_to_resource_[real_path] = shared;

		return shared;
	};

	LoadMode load_mode = LoadMode::Directory;
private:
	struct AssetData {
		std::string path = "";
		std::string absolute_path = "";
		uintmax_t offset = 0;
		uintmax_t size = 0;
	};

	void* PtrFromFile(std::string file_path, std::string extension);
	void* PtrFromPack(std::string asset_path, std::string extension);
	void AddAssetData(const AssetData& asset_path_data);
	
	static constexpr std::string_view kAssetListStartMarker = "file start";

	std::map<std::string, std::unique_ptr<ResourceFormatLoader>> extension_to_format_loader_ = { };
	std::map<std::string, std::weak_ptr<void>> file_path_to_resource_ = { };
	
	std::filesystem::path pack_path_;
	std::map<std::string, AssetData> asset_data_map_ = {};
	std::vector<AssetData> assets_data_ = {};
};





