#include "fmod_bank_loader.h"
#include "../engine.h"

void* FmodBankLoader::LoadFromFile(std::string file_path) {
	FMOD::Studio::Bank* bank;
	FMOD_RESULT err = Engine::GetFmodSystem()->loadBankFile(
		file_path.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);
	bank->loadSampleData();

	return bank;
}

void* FmodBankLoader::LoadFromMemory(void* ptr, uintmax_t len) {
	FMOD::Studio::Bank* bank;
	FMOD_RESULT err = Engine::GetFmodSystem()->loadBankMemory(
		reinterpret_cast<char*>(ptr),
		len,
		FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY,
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	return bank;
}

std::vector<std::string> FmodBankLoader::GetSupportedExtensions() {
	return { ".bank" };
}
