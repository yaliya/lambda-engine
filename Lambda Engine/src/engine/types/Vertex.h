#pragma once

#include <glm\glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 texcoords;

	Vertex();
	Vertex(const glm::vec3& position);
	Vertex(const glm::vec3& position, const glm::vec3& normals);
	Vertex(const glm::vec3& position, const glm::vec3& normals, const glm::vec2& texcoords);
};