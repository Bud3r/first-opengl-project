#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"
#include "glad/glad.h"


class Texture
{
public:
	static Texture* Load(const char* filePath);
	unsigned int GetId();
private:
	Texture(const char* filePath);
	~Texture();
	unsigned int textureID;
	std::string m_filePath;
	static std::vector<Texture*> loadedTextures;
};


Texture* Texture::Load(const char* filePath) {
	for (Texture* texture : loadedTextures) {
		if (std::strcmp(texture->m_filePath.c_str(), filePath) == 0) {
			return texture;
		}
	}

	Texture* texture = new Texture(filePath);
	loadedTextures.push_back(texture);
	
	return texture;
}

Texture::Texture(const char* p_filePath)
{
	m_filePath = p_filePath;
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

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

	loadedTextures.push_back(this);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
	std::erase(loadedTextures, this);
}

unsigned int Texture::GetId() {
	return textureID;
}