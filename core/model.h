#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "helper.h"
#include "texture.h"
#include "shader_program.h"
#include "mesh.h"

class Model
{
public:
	Model() { };
	Model(const char* file_path);
	Model(const void* ptr, size_t len);
	Model(Mesh* mesh) {
		meshes_.push_back(mesh);
	}
	void Draw(const ShaderProgram& program, glm::mat4 model_matrix, glm::vec4 modulate = glm::vec4(1.0));
	glm::mat4 static GetModelMatrix(glm::vec3 position, glm::vec3 rotation);
private:
	void ParseNode(aiNode* node, const aiScene* scene);
	Mesh* ParseMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name, const aiScene* scene);
	std::vector<Mesh*> meshes_;
};