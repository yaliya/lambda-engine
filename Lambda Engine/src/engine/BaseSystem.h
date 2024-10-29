#pragma once

#include <string>
#include <vector>
#include <memory>

#include "managers/EntityManager.h"

class BaseSystem
{
public:
	virtual void Update(const std::shared_ptr<EntityManager>& world, float delta) = 0;
};