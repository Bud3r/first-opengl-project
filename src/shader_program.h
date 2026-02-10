#pragma once

#include <fmt/core.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

constexpr int InfoLogSize = 512;

class ShaderProgram
{
public:
	ShaderProgram() { };
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();
	void Load(const char* vertexPath, const char* fragmentPath);
	void use() const;
	int getId() const;
	GLint getLocation(const char* uniform) const;
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
private:
	GLuint ID;
};