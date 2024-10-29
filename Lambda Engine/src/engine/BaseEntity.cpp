#include "BaseEntity.h"

BaseEntity::BaseEntity()
{
	enabled = true;
}

BaseEntity::~BaseEntity()
{
}

BaseEntity* BaseEntity::SetName(std::string name)
{
	this->name = name;

	return this;
}