#pragma once

#include <iostream>
#include <array>
#include <vector>

class Texture
{
public:
	unsigned int GetId();
	static Texture* FromFile(const char* file_path);
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