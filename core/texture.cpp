#include "texture.h"

#include "stb_image.h"

Texture* Texture::Load(const char* filePath) {
	Texture* texture = new Texture(filePath);
	return texture;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

Texture::Texture(const char* p_filePath)
{
	m_filePath = p_filePath;
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels_in_file;
	stbi_uc* data = stbi_load(p_filePath, &width, &height, &channels_in_file, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture: " << p_filePath << std::endl;
	}
}

Texture* Texture::FromMemory(stbi_uc* start, int len) {
	int width, height, channels_in_file;
	stbi_uc* data = stbi_load_from_memory(start, len, &width, &height, &channels_in_file, 0);
	return Texture::FromData(data, width, height, channels_in_file);
}

Texture* Texture::FromData(stbi_uc* data, int width, int height, int channels_in_file) {
	if (data != nullptr) {
		GLenum texture_type;
		switch (channels_in_file)
		{
			case 1:
				texture_type = GL_R8;
				break;
			case 2:
				texture_type = GL_RG8;
				break;
			case 3:
				texture_type = GL_RGB8;
				break;
			case 4:
				texture_type = GL_RGBA8;
				break;
			default:
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, texture_type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cerr << stbi_failure_reason() << std::endl;
		throw std::invalid_argument("Argument data pointer is nullptr.");
	}
}

unsigned int Texture::GetId() {
	return m_id;
}