#pragma once

#include <string>

class BaseAsset
{
public:
	bool loaded;
	std::string filename;
	virtual ~BaseAsset() = default;
	virtual bool load() = 0;
};