#pragma once

#include <vector>

#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\archives\json.hpp>

#include "../BaseEntity.h"

class EntityManager
{
private:
	int entityId;
	std::shared_ptr<BaseEntity> lastSelectedEntity;
	std::vector<std::shared_ptr<BaseEntity>> entities;
public:
	EntityManager();
	virtual ~EntityManager();

	std::shared_ptr<BaseEntity> CreateEntity();
	std::shared_ptr<BaseEntity> GetLastSelectedEntity();
	std::shared_ptr<BaseEntity> GetEntityByName(std::string name);
	void SetLastSelectedEntity(std::shared_ptr<BaseEntity> entity);
	std::vector<std::shared_ptr<BaseEntity>> GetEntities();

	template <typename... T>
	void EachEntityWithComponent(typename std::common_type <std::function<void(BaseEntity&, T&...)>> ::type callback)
	{
		for (auto it : entities)
		{
			if (it->HasComponent<T...>())
			{
				callback(*it, *it->GetComponent<T...>());
			}
		}
	}

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(entityId), CEREAL_NVP(entities));
	}
};