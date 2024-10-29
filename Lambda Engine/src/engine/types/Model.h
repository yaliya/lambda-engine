#pragma once

#include <iostream>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include "../BaseAsset.h"
#include "../types/Mesh.h"
#include "../managers/AssetManager.h"

class Model : public BaseAsset
{
private:
	std::string directory;
	std::vector<Mesh> meshes;
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadTextures(aiMaterial *material, aiTextureType type);
public:
	Model(const std::string& filename);
	std::vector<Mesh> getMeshes();
	virtual ~Model();
	bool load();
};