#pragma once

#include <iostream>
#include <vector>
#include "glad/glad.h"

class Texture
{
public:
	~Texture();

	static Texture* Load(const char* p_file_path);
	unsigned int GetId();
	
	std::string type;
private:
	Texture(const char* p_file_path);

	unsigned int m_id;

	std::string m_filePath;
};