#pragma once

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <unordered_map>

#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

#include "BaseComponent.h"


class BaseEntity
{
private:
	std::unordered_map<std::string, std::shared_ptr<BaseComponent>> components;

public:
	bool enabled;
	std::string name;
	unsigned int id;

	BaseEntity();
	virtual ~BaseEntity();
	BaseEntity* SetName(std::string name);

	void EachComponent(typename std::common_type<std::function<void(BaseComponent&, BaseEntity&)>>::type callback)
	{
		for (auto it : components)
		{
			callback(*it.second, *this);
		}
	}
	
	template<typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&& ...args)
	{
		auto component = std::make_shared<T>(args...);
		components.insert(std::make_pair(typeid(T).name(), std::move(component)));
		return component;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		if (HasComponent<T>())
		{
			auto index = typeid(T).name();
			return std::static_pointer_cast<T>(components.find(index)->second);
		}
		
		return nullptr;
	}

	template<typename T>
	bool HasComponent() const
	{
		auto index = typeid(T).name();
		return components.find(index) != components.end();
	}

	template<typename T, typename V, typename... Types>
	bool HasComponent() const
	{
		return HasComponent<T>() && HasComponent<V, Types...>();
	}

	template <typename... T>
	void EachComponent(typename std::common_type<std::function<void(std::shared_ptr<T>...)>>::type callback) 
	{
		if (HasComponent<T...>()) 
		{
			callback(GetComponent<T...>());
		}
	}

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(enabled), CEREAL_NVP(components));
	}
};