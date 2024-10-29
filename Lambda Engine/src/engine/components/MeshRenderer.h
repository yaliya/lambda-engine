#pragma once

#include <GL\glew.h>
#include "../types/Model.h"
#include "../BaseComponent.h"
#include "../types/Mesh.h"

struct MeshRenderer : BaseComponent
{
	std::string filename;
	std::vector<Mesh> meshes;
	std::shared_ptr<Model> model;

	MeshRenderer(std::string filename);
	MeshRenderer(std::shared_ptr<Model> model);
	MeshRenderer(const std::vector<Mesh>& meshes);

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(cereal::make_nvp("filename", model->filename));
	}

	
	template<class Archive>
	static void load_and_construct(Archive & ar, cereal::construct<MeshRenderer> & construct)
	{
		std::string filename;

		ar(filename);
		construct(filename);
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseComponent, MeshRenderer);