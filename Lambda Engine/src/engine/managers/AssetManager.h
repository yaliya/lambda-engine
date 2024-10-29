#pragma once

#include <memory>
#include <unordered_map>
#include "../BaseAsset.h"
#include "../utils/Assert.h"

namespace AssetManager
{
	extern std::unordered_map<std::string, std::shared_ptr<BaseAsset>> assets;

	template <typename T, typename ...Args>
	std::shared_ptr<T> LoadAsset(const std::string& filename, Args... args)
	{
		auto asset = std::make_shared<T>(filename, args...);

		if (asset->load())
		{
			std::cout << "Loaded asset " << filename << " from disk ! " << std::endl;

			assets.insert(std::make_pair(filename, std::move(asset)));

			return std::static_pointer_cast<T>(assets.find(filename)->second);
		}

		return nullptr;
	}


	template <typename T, typename ...Args>
	std::shared_ptr<T> GetAsset(const std::string& filename, Args... args)
	{
		if (!AssetLoaded(filename))
		{
			return LoadAsset<T>(filename, args...);
		}

		return std::static_pointer_cast<T>(assets.find(filename)->second);
	}

	bool AssetLoaded(std::string filename);
};