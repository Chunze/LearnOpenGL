﻿#include <iostream>

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"


Model::Model(GameContext* gameContext, char *path)
	: Super(gameContext)
{
	LoadModel(path);
	m_Shader = new Shader("SimpleWorld/SimpleShader.vert", "SimpleWorld/SimpleShader.frag");
	m_Shader->SetUniformFloat("material.shininess", 2.0f);
}

Model::~Model()
{
}

std::vector<DrawListElement>& Model::GetDrawListElement()
{
	if (bDirty)
	{
		for (Mesh& mesh : m_Meshes)
		{
			auto element = mesh.GetDrawListElement();
			element.shader = *m_Shader;
			m_DrawListElements.push_back(element);
		}

		bDirty = false;
	}

	return m_DrawListElements;
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (&mesh->mNormals[0])
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}
		else
		{
			vertex.Normal = glm::vec3(0.0f);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// Loading diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, str_TEXTURE_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// Loading specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, str_TEXTURE_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// loading normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, str_TEXTURE_NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// loading height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, str_TEXTURE_HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(m_gameContext, vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_TextureLoaded.size(); j++)
		{
			if (std::strcmp(m_TextureLoaded[j].GetFileName().data(), str.C_Str()) == 0)
			{
				textures.push_back(m_TextureLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   
			// if texture hasn't been loaded already, load it
			Texture* texture = new Texture(std::string(str.C_Str()), m_Directory);
			texture->m_Type = typeName;
			textures.push_back(*texture);
			m_TextureLoaded.push_back(*texture); // add to loaded textures
		}
	}
	return textures;
}
