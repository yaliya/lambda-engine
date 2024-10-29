#pragma once

#include <string>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>

#include <glm/glm.hpp>

namespace glm
{
	template<typename Archive>
	void serialize(Archive& archive, glm::vec3& v3)
	{
		archive(cereal::make_nvp("x", v3.x), cereal::make_nvp("y", v3.y), cereal::make_nvp("z", v3.z));
	}

	template<typename Archive>
	void serialize(Archive& archive, glm::vec2& v2)
	{
		archive(cereal::make_nvp("x", v2.x), cereal::make_nvp("y", v2.y));
	}
}

struct BaseComponent
{
	bool enabled;
	std::string name;
	
	BaseComponent() { enabled = true; };
	virtual ~BaseComponent() = default;

	template<class Archive>
	void serialize(Archive &archive) {}
};