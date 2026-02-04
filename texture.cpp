#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"


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
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels_in_file;
	unsigned char* data = stbi_load(p_filePath, &width, &height, &channels_in_file, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture: " << p_filePath << std::endl;
	}
}

unsigned int Texture::GetId() {
	return m_id;
}