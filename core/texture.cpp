#include "texture.h"

#include "stb_image.h"
#include <array>

Texture::Texture() {
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
	glDeleteTextures(1, &id_);
}

Texture* Texture::Load(const char* file_path) {
	int width, height, channels_in_file;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(file_path, &width, &height, &channels_in_file, 0);
	Texture* texture = Texture::FromData(data, width, height, channels_in_file);
	stbi_image_free(data);

	texture->file_path = file_path;

	return texture;
}

Texture* Texture::FromMemory(unsigned char* start, int len) {
	int width, height, channels_in_file;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load_from_memory(start, len, &width, &height, &channels_in_file, 0);
	Texture* texture = Texture::FromData(data, width, height, channels_in_file);
	stbi_image_free(data);
	return texture;
}

Texture* Texture::FromData(unsigned char* data, int width, int height, int channels_in_file) {
	if (data == nullptr) {
		std::cerr << stbi_failure_reason() << std::endl;
		throw std::invalid_argument("Data pointer is nullptr.");
	}

	constexpr std::array<GLenum, 4> kTextureTypes = { GL_R, GL_RG, GL_RGB, GL_RGBA };
	GLenum texture_type = kTextureTypes[channels_in_file - 1];

	Texture* texture = new Texture();

	glTexImage2D(GL_TEXTURE_2D, 0, texture_type, width, height, 0, texture_type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

unsigned int Texture::GetId() {
	return id_;
}