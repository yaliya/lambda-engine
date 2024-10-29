#pragma once

#include "../BaseComponent.h"

#include <glm/glm.hpp>

struct Transform : BaseComponent
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	float Axis;

	Transform();
	Transform(glm::vec3& position);
	Transform(glm::vec3& position, glm::vec3& rotation);
	Transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(name), CEREAL_NVP(enabled), CEREAL_NVP(Position), CEREAL_NVP(Rotation), CEREAL_NVP(Scale));
	}
};

CEREAL_REGISTER_TYPE(Transform);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseComponent, Transform);