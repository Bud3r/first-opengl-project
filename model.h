#pragma once

#include "mesh.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char* filePath);
	~Model();
	void Draw(const ShaderProgram& program);
	std::vector<Mesh> meshes;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec4 m_modulate = vec4(4.0);
private:
	void parseNode(aiNode* node, const aiScene* scene);
	Mesh parseMesh(aiMesh* mesh, const aiScene* scene);
};

Model::Model(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene ->mRootNode) {
		std::printf(importer.GetErrorString());
		return;
	}

	parseNode(scene->mRootNode, scene);
}

Model::~Model()
{
}

void Model::Draw(const ShaderProgram &program) {
	mat4 model = mat4(1.0f);
	model = glm::rotate(model, m_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, m_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, m_rotation.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, m_position);
	program.use();
	glUniform4fv(program.getLocation("modulate"), 1, value_ptr(m_modulate));
	glUniformMatrix4fv(program.getLocation("model"), 1, GL_FALSE, value_ptr(model));
	for (auto mesh : meshes) {
		mesh.Draw();
	}
}

void Model::parseNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		meshes.push_back(parseMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		parseNode(node->mChildren[i], scene);
	}
}

Mesh Model::parseMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture*> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{
			glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
		};
		
		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices, textures);
}