#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"
#include "glad/glad.h"

class Texture
{
public:
	Texture(const char* filePath);
	~Texture();
	unsigned int GetId();
private:
	unsigned int textureID;
};

Texture::Texture(const char* filePath)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels_in_file;
	unsigned char* data = stbi_load(filePath, &width, &height, &channels_in_file, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture: " << filePath << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

unsigned int Texture::GetId() {
	return textureID;
}