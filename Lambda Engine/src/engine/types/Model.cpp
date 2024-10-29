#include "Model.h"

Model::Model(const std::string& filename)
{
	this->filename = filename;
}

Model::~Model()
{
}

std::vector<Mesh> Model::getMeshes()
{
	return meshes;
}

bool Model::load()
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;

		return false;
	}

	directory = filename.substr(0, filename.find_last_of('\\'));

	processNode(scene->mRootNode, scene);

	return true;
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(Vertex(

			glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),

			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),

			glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y)
		));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		auto& face = mesh->mFaces[i];
	
		for (unsigned j = 0; j < face.mNumIndices; j++)
		{
			int indice = face.mIndices[j];

			indices.push_back(indice);
		}
	}

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
	std::vector<std::shared_ptr<Texture>> specularMaps = loadTextures(material, aiTextureType_SPECULAR);

	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return Mesh(vertices, indices, textures);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

std::vector<std::shared_ptr<Texture>> Model::loadTextures(aiMaterial * material, aiTextureType type)
{
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;

		material->GetTexture(type, i, &str);

		TextureType textureType;

		if (type == aiTextureType_DIFFUSE)
		{
			textureType = TextureType::DIFFUSE_MAP;
		}

		else if (type == aiTextureType_SPECULAR)
		{
			textureType = TextureType::SPECULAR_MAP;
		}

		auto texture = AssetManager::GetAsset<Texture>(directory + "\\" + str.C_Str(), textureType);

		if (texture != nullptr)
		{
			textures.push_back(texture);
		}
	}

	return textures;
}
