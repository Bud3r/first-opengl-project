#pragma once

#include <iostream>
#include <vector>
#include "glad/glad.h"


class Texture
{
public:
	unsigned int GetId();
	static Texture* Load(const char* file_path);
	static Texture* FromMemory(unsigned char* start, int len);
	static Texture* FromData(unsigned char* data, int width, int height, int channels_in_file);

	~Texture();

	std::string type;

private:
	Texture();
	Texture(const char* file_path);
	unsigned int id_;

	std::string file_path;
};