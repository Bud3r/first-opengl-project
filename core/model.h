#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "resource_loader.h"
#include "texture.h"
#include "shader_program.h"
#include "mesh.h"

class Model
{
public:
	Model(const char* filePath);
	Model() { };
	Model(Mesh* mesh) {
		meshes.push_back(mesh);
	}
	void Draw(const ShaderProgram& program, glm::mat4 model_matrix, glm::vec4 p_modulate = glm::vec4(1.0));
	glm::mat4 static create_model_matrix(glm::vec3 p_position, glm::vec3 p_rotation);
	std::vector<Mesh*> meshes;
private:
	void parseNode(aiNode* node, const aiScene* scene);
	Mesh* parseMesh(aiMesh* mesh, const aiScene* scene);
};