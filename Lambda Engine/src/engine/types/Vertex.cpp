#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(const glm::vec3& position)
{
	this->position = position;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normals)
{
	this->normals = normals;
	this->position = position;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normals, const glm::vec2& texcoords)
{
	this->normals = normals;
	this->position = position;
	this->texcoords = texcoords;
}