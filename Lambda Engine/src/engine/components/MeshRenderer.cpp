#include "MeshRenderer.h"
#include "../managers/AssetManager.h"

MeshRenderer::MeshRenderer(std::string filename)
{
	filename = filename;
	name = "MeshRenderer";
	model = AssetManager::GetAsset<Model>(filename);
	meshes = model->getMeshes();
}

MeshRenderer::MeshRenderer(std::shared_ptr<Model> model)
{
	this->model = model;
	name = "MeshRenderer";
	filename = model->filename;
	meshes = model->getMeshes();
}

MeshRenderer::MeshRenderer(const std::vector<Mesh>& meshes)
{
	name = "MeshRenderer";
	this->meshes = meshes;
}
