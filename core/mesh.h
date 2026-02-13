#pragma once

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "texture.h"
#include "shader_program.h"


struct Vertex
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec2 tex_coords = glm::vec2(0.0f);
};

class Mesh
{
 public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);
	void Draw(const ShaderProgram& program);
	
 private:
	std::vector<Vertex> vertices_;
	std::vector<uint32_t> indices_;
	std::vector<std::shared_ptr<Texture>> textures_;

	GLuint vbo_;
	GLuint vao_; 
	GLuint ebo_;
};

