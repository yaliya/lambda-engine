#include "Transform.h"

Transform::Transform()
{
	name = "Transform";
	this->Position = glm::vec3(0, 0, 0);
	this->Rotation = glm::vec3(0, 1, 0);
	this->Scale = glm::vec3(1, 1, 1);
	this->Axis = 0.0f;
}

Transform::Transform(glm::vec3& position)
{
	name = "Transform";
	this->Position = position;
	this->Rotation = glm::vec3(0, 1, 0);
	this->Scale = glm::vec3(1, 1, 1);
	this->Axis = 0.0f;
}

Transform::Transform(glm::vec3& position, glm::vec3& rotation)
{
	name = "Transform";
	this->Position = position;
	this->Rotation = rotation;
	this->Scale = glm::vec3(1, 1, 1);
	this->Axis = 0.0f;
}

Transform::Transform(glm::vec3 & position, glm::vec3 & rotation, glm::vec3 & scale)
{
	name = "Transform";
	this->Position = position;
	this->Rotation = rotation;
	this->Scale = scale;
	this->Axis = 0.0f;
}
