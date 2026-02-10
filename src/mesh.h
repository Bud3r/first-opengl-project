#pragma once

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "texture.h"

struct Vertex
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Normal = glm::vec3(0.0f);
	glm::vec2 TexCoords = glm::vec2(0.0f);
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> p_vertices, std::vector<uint32_t> p_indices, std::vector<Texture*> p_textures);
	void Draw();
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture*> textures;
private:
	GLuint VBO, VAO, EBO;
};

