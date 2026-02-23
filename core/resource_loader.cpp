#include "resource_loader.h"

#include "resource_loaders/texture_loader.h"
#include "resource_loaders/model_loader.h"
#include "resource_loaders/shader_program_loader.h"
#include "engine.h"

ResourceLoader::ResourceLoader()
{
	std::filesystem::path exec_path = Engine::GetExecutablePath();

	AddResourceFormatLoader<TextureLoader>();
	AddResourceFormatLoader<ModelLoader>();
	AddResourceFormatLoader<ShaderProgramLoader>();
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

void* ResourceLoader::PtrFromFile(std::filesystem::path file_path, std::filesystem::path extension) {
	file_path = get_real_file_path(file_path);
	if (!std::filesystem::exists(file_path)) {
		throw std::logic_error("File '" + file_path.string() + "' does not exist.");
	}

	return extension_to_format_loader_[extension]->LoadFromFile(file_path.string());
}

void* ResourceLoader::PtrFromPack(std::filesystem::path asset_path, std::filesystem::path extension) {
	if (!AssetExists(asset_path)) {
		throw std::logic_error("Asset '" + asset_path.string() + "' does not exist.");
	}

	std::vector<char> data = GetAssetData(asset_path);

	return extension_to_format_loader_[extension]->LoadFromMemory(&data[0], data.size());
}


const std::vector<char> ResourceLoader::GetAssetData(std::filesystem::path asset_path) {
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

	asset_pack.seekg(asset_data.offset + header_offset_);
	asset_pack.read(&data[0], asset_data.size);

	// TODO: It copies the vector
	return data;
}


void ResourceLoader::SaveAssetPack(const std::filesystem::path& assets_directory_path, const std::filesystem::path& save_path) {
	std::ofstream asset_pack(save_path, std::ios::binary);
	std::filesystem::recursive_directory_iterator assets_iterator(assets_directory_path);

	std::filesystem::path assets_path = std::filesystem::path(ASSETS_PATH);
	std::filesystem::path generated_assets_path = std::filesystem::path(GENERATED_ASSETS_PATH);

	uintmax_t file_offset = 0;

	// Create pack header.
	for (std::filesystem::directory_entry file : assets_iterator) {
		if (!file.is_regular_file()) {
			continue;
		}

		if (!CanLoad(file.path())) {
			continue;
		}

		std::filesystem::path relative_path =
			std::filesystem::relative(file.path(), assets_directory_path);
		std::filesystem::path absulute_path;

		std::ifstream asset_file;

		if (extension_to_format_loader_[file.path().extension()]->ShouldCreateGeneratedAsset()) {
			std::filesystem::path generated_asset_path;
			generated_asset_path =
				generated_assets_path / std::filesystem::relative(file.path(), ASSETS_PATH);
			std::cout << "Generated works: " << std::filesystem::create_directories(generated_asset_path.parent_path()) << std::endl;
			std::ifstream base_file(file.path());
			std::ofstream generated_file(generated_asset_path);
			extension_to_format_loader_[file.path().extension()]->CreateGeneratedAsset(
				base_file,
				generated_file,
				file.path()
			);
			asset_file = std::ifstream(generated_asset_path);
			absulute_path = generated_asset_path;
		} else {
			asset_file = std::ifstream(file.path());
			absulute_path = file.path();
		}
		asset_file.seekg(0, std::ios_base::end);
		AssetData data = { relative_path.generic_string(), absulute_path.string(), file_offset , asset_file.tellg() };
		asset_pack << data.path.string() << "\n" << data.offset << "\n" << data.size << "\n";
		AddAssetData(data);
		file_offset += data.size;
	}

	asset_pack << kAssetListStartMarker << std::endl;

	for (AssetData data : assets_data_) {
		std::ifstream asset_file; 
		
		if (extension_to_format_loader_[data.path.extension()]->ShouldCreateGeneratedAsset()) {
			asset_file = std::ifstream(data.absolute_path, std::ios::binary);
		} else {
			asset_file = std::ifstream(data.absolute_path, std::ios::binary);
		}
		asset_file = std::ifstream(data.absolute_path, std::ios::binary);
		asset_pack << asset_file.rdbuf();
		asset_file.close();
		std::cerr << "Inserted file: " << data.path.string() << " Offset: " << data.offset << " Size: " << data.size << std::endl;
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
	int asset_count = 0;

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
		asset_count++;

		if (asset_count > kMaxAssetCount) {
			throw std::logic_error("More than 100000 assets, parser parsed incorrectly or you just have a lot of assets.");
		}
	}

	header_offset_ = asset_pack.tellg();

	pack_path_ = load_path;
	load_mode = LoadMode::AssetPack;
	std::cout << "Loaded pack: " << load_path << " Header offset: " << header_offset_ << std::endl;
}

bool ResourceLoader::AssetExists(const std::filesystem::path& asset_path) {
	return asset_data_map_.contains(asset_path);
}

void ResourceLoader::AddAssetData(const AssetData& asset_path_data) {
	asset_data_map_[asset_path_data.path] = asset_path_data;
	assets_data_.push_back(asset_path_data);
}
