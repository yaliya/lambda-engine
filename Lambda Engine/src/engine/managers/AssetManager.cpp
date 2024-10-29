#include "AssetManager.h"

namespace AssetManager
{
	std::unordered_map<std::string, std::shared_ptr<BaseAsset>> assets;

	bool AssetLoaded(std::string filename)
	{
		return assets.count(filename) > 0;
	}
}
