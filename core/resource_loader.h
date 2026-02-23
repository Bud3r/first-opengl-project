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
	const std::vector<char> GetAssetData(std::filesystem::path asset);
	void SaveAssetPack(const std::filesystem::path& assets_directory_path, const std::filesystem::path& save_path);
	void LoadAssetPack(const std::filesystem::path& load_path);
	bool AssetExists(const std::filesystem::path& asset);

	template<typename T>
	requires std::is_base_of_v<ResourceFormatLoader, T>
	void AddResourceFormatLoader() {
		std::unique_ptr<T> format_loader = std::make_unique<T>();

		for (std::string extension : format_loader->GetSupportedExtensions()) {
			extension_to_format_loader_[extension] = std::make_unique<T>();
		}
	}

	template<typename T>
	std::shared_ptr<T> Load(std::filesystem::path asset_path) {
		void* ptr = 0;

		if (file_path_to_resource_.contains(asset_path)) {
			std::weak_ptr<void> weak = file_path_to_resource_[asset_path];

			if (!weak.expired()) {
				return std::static_pointer_cast<T>(weak.lock());
			}
		}

		switch (load_mode)
		{
			case LoadMode::Directory:
			ptr = PtrFromFile(asset_path, asset_path.extension());
			break;
			case LoadMode::AssetPack:
			ptr = PtrFromPack(asset_path, asset_path.extension());
			break;
			default:
			break;
		}

		if (ptr == nullptr) {
			throw std::logic_error("Loader failed to load file " + asset_path.string());
		}

		std::cout << "Loaded: " + asset_path.string() << std::endl;

		std::shared_ptr<T> shared((T*)ptr);
		file_path_to_resource_[asset_path] = shared;

		return shared;
	};

	LoadMode load_mode = LoadMode::Directory;
private:
	struct AssetData {
		std::filesystem::path path = "";
		std::filesystem::path absolute_path = "";
		uintmax_t offset = 0;
		uintmax_t size = 0;
	};

	void* PtrFromFile(std::filesystem::path asset_path, std::filesystem::path extension);
	void* PtrFromPack(std::filesystem::path asset_path, std::filesystem::path extension);
	void AddAssetData(const AssetData& asset_path_data);
	
	static constexpr std::string_view kAssetListStartMarker = "file start";
	size_t header_offset_ = 0;

	std::map<std::filesystem::path, std::unique_ptr<ResourceFormatLoader>> extension_to_format_loader_ = { };
	std::map<std::filesystem::path, std::weak_ptr<void>> file_path_to_resource_ = { };

	std::filesystem::path pack_path_;
	std::map<std::filesystem::path, AssetData> asset_data_map_ = {};
	std::vector<AssetData> assets_data_ = {};
};





