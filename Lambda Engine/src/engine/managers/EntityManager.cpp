#include "EntityManager.h"

EntityManager::EntityManager()
{
	entityId = 0;
}

EntityManager::~EntityManager()
{
}

std::shared_ptr<BaseEntity> EntityManager::CreateEntity()
{
	auto entity = std::make_shared<BaseEntity>();

	entity->id = ++entityId;
	entity->name = "GameObject" + std::to_string(entity->id);
	entities.push_back(std::move(entity));

	return entities.back();
}

std::shared_ptr<BaseEntity> EntityManager::GetEntityByName(std::string name)
{
	for (auto it : entities)
	{
		if (it->name == name)
		{
			return it;
		}
	}

	return nullptr;
}

std::shared_ptr<BaseEntity> EntityManager::GetLastSelectedEntity()
{
	return lastSelectedEntity;
}

void EntityManager::SetLastSelectedEntity(std::shared_ptr<BaseEntity> entity)
{
	lastSelectedEntity = entity;
}

std::vector<std::shared_ptr<BaseEntity>> EntityManager::GetEntities()
{
	return entities;
}
