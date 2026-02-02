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
	void parseNode(aiNode* node, const aiScene* scene);
	void parseMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh> meshes;

private:

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

void Model::parseNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		parseMesh(mesh, scene);
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		parseNode(node->mChildren[i], scene);
	}
}

void Model::parseMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	//std::vector<Texture> textures;

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
	
	if (mesh->mMaterialIndex < scene->mNumMaterials) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString str;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		Texture texture;
		//texture.id =
	}
}