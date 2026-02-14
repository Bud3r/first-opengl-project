#include "model.h"


Model::Model(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << importer.GetErrorString() << std::endl;
		return;
	}

	ParseNode(scene->mRootNode, scene);
}


glm::mat4 Model::GetModelMatrix(glm::vec3 p_position, glm::vec3 p_rotation) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, p_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, p_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, p_rotation.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, p_position);
	return model;
}


void Model::Draw(const ShaderProgram& program, glm::mat4 model_matrix, glm::vec4 p_modulate) {
	program.Use();
	glUniform4fv(program.GetLocation("modulate"), 1, glm::value_ptr(p_modulate));
	glUniformMatrix4fv(program.GetLocation("model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	for (auto mesh : meshes_) {
		mesh->Draw(program);
	}
}

void Model::ParseNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		meshes_.push_back(ParseMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ParseNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ParseMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{
			glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
		};

		if (mesh->mTextureCoords[0]) {
			vertex.tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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
	
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> albedo_texture = LoadMaterialTextures(material, aiTextureType_BASE_COLOR, "albedo", scene);
		textures = albedo_texture;
	}

	return new Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(
	aiMaterial* material, aiTextureType type, 
	std::string type_name, const aiScene* scene) {

	std::vector<std::shared_ptr<Texture>> textures;

	for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		const aiTexture* ai_texture = scene->GetEmbeddedTexture(str.C_Str());
		Texture* tex_ptr = Texture::FromMemory(reinterpret_cast<unsigned char*>(ai_texture->pcData), ai_texture->mWidth);
		tex_ptr->type = type;
		std::shared_ptr<Texture> texture(tex_ptr);
		textures.push_back(texture);
	}

	return textures;
}