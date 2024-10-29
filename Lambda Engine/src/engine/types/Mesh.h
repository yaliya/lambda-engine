#pragma once

#include <GL\glew.h>
#include <cereal/types/vector.hpp>

#include "Vertex.h"
#include "Texture.h"

struct Mesh
{
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	void compile();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
};