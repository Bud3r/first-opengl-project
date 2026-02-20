#include "texture_loader.h"
#include "../shader_program.h"


class ShaderProgramLoader : public ResourceFormatLoader {
public:
	virtual void* LoadFromFile(std::string file_path) override;
	virtual void* LoadFromMemory(void* ptr, uintmax_t len) override;
	virtual std::vector<std::string> GetSupportedExtensions() override;
};