#include "resource_loader.h"

#include "resource_loaders/texture_loader.h"
#include "resource_loaders/model_loader.h"
#include "engine.h"

ResourceLoader::ResourceLoader()
{
	std::filesystem::path exec_path = Engine::GetExecutablePath();

	AddResourceFormatLoader<TextureLoader>();
	AddResourceFormatLoader<ModelLoader>();
}

bool ResourceLoader::CanLoad(std::filesystem::path path) const {
	if (!std::filesystem::exists(path)) {
		return false;
	}

	if (!CanLoadExtension(path.extension())) {
		return false;
	}

	return true;
}

bool ResourceLoader::CanLoadExtension(std::filesystem::path extension) const {
	if (!extension_to_format_loader_.contains(extension.string())) {
		return false;
	}

	return true;
}

void* ResourceLoader::PtrFromFile(std::string file_path, std::string extension) {
	if (!std::filesystem::exists(file_path)) {
		throw std::logic_error("File '" + file_path + "' does not exist.");
	}

	return extension_to_format_loader_[extension]->LoadFromFile(file_path);
}

void* ResourceLoader::PtrFromPack(std::string asset_path, std::string extension) {
	if (!AssetExists(asset_path)) {
		throw std::logic_error("Asset '" + asset_path + "' does not exist.");
	}

	std::vector<char> data = GetAssetData(asset_path);

	return extension_to_format_loader_[extension]->LoadFromMemory(&data[0], data.size());
}


const std::vector<char> ResourceLoader::GetAssetData(std::string asset_path) {
	assert(std::filesystem::exists(pack_path_) && "Asset pack file path doesn't exit, make sure you load the pack before calling this.");

	if (!asset_data_map_.contains(asset_path)) {
		throw std::invalid_argument("Asset path not in pack");
	}

	AssetData asset_data = asset_data_map_[asset_path];

	for (AssetData d : assets_data_) {
		if (d.path == asset_path) {
			asset_data = d;
			break;
		}
	}

	std::ifstream asset_pack(pack_path_, std::ios::binary);

	std::vector<char> data;
	data.resize(asset_data.size);

	asset_pack.seekg(asset_data.offset);
	asset_pack.read(&data[0], asset_data.size);

	// TODO: It copies the vector
	return data;
}


void ResourceLoader::SaveAssetPack(const std::filesystem::path& assets_directory_path, const std::filesystem::path& save_path) {
	std::ofstream asset_pack(save_path, std::ios::binary);
	std::filesystem::recursive_directory_iterator assets_iterator(assets_directory_path);

	uintmax_t file_offset = 0;

	for (std::filesystem::directory_entry file : assets_iterator) {
		if (!file.is_regular_file()) {
			continue;
		}

		if (!CanLoad(file.path())) {
			continue;
		}

		// {path}
		// {offset}{size}
		// {files ...}

		std::filesystem::path relative_path = std::filesystem::relative(file.path(), assets_directory_path);
		std::ifstream asset_file(file.path());
		AssetData data = { relative_path.string(), file.path().string(), file_offset , file.file_size() };
		asset_pack << data.path.c_str() << "\n" << data.offset << "\n" << data.size << "\n";
		AddAssetData(data);
		file_offset += file.file_size();
	}

	asset_pack << kAssetListStartMarker << std::endl;

	for (AssetData data : assets_data_) {
		std::ifstream asset_file(data.absolute_path, std::ios::binary);
		asset_pack << asset_file.rdbuf();
		asset_file.close();
		std::cerr << "Inserted file: " << data.path << " Offset: " << data.offset << " Size: " << data.size << std::endl;
	};

	asset_pack.close();
}

void ResourceLoader::LoadAssetPack(const std::filesystem::path& load_path) {
	std::ifstream asset_pack(load_path, std::ios::binary);

	if (!asset_pack.is_open()) {
		throw std::logic_error("Can't open asset pack: " + load_path.string());
	}

	std::string line;

	const int kMaxAssetCount = 100000;
	int assetCount = 0;

	while (true)
	{
		AssetData data = { };

		std::getline(asset_pack, line);
		// TODO: Clean this break.
		if (line == kAssetListStartMarker) {
			break;
		}

		data.path = line;
		asset_pack >> data.offset;
		asset_pack >> data.size;
		asset_pack.ignore(1);
		AddAssetData(data);
		std::cout << "Loaded asset: " << data.path << ", Offset: " << data.offset << ", Size: " << data.size << std::endl;
		assetCount++;

		if (assetCount > kMaxAssetCount) {
			throw std::logic_error("More than 100000 assets, parser parsed incorrectly or you just have a lot of assets.");
		}
	}

	pack_path_ = load_path;
	load_mode = LoadMode::AssetPack;
	std::cout << "Loaded pack: " << load_path << std::endl;
}

bool ResourceLoader::AssetExists(std::string asset) {
	return asset_data_map_.contains(asset);
}

void ResourceLoader::AddAssetData(const AssetData& asset_path_data) {
	asset_data_map_[asset_path_data.path] = asset_path_data;
	assets_data_.push_back(asset_path_data);
}
