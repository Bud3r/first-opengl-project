#pragma once

#include <fmt/core.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

constexpr int kInfoLogSize = 512;

class ShaderProgram
{
public:
	ShaderProgram() { };
	ShaderProgram(const char* vertex_path, const char* fragment_path);
	~ShaderProgram();
	void Load(const char* vertex_path, const char* fragment_path);
	void Use() const;
	int GetId() const;
	GLint GetLocation(const char* uniform) const;
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
private:
	GLuint id_ = 0;
};