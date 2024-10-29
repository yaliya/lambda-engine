#pragma once

#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>

#include "../BaseAsset.h"
#include "../utils/stb_image.h"

enum TextureType
{
	DIFFUSE_MAP,
	NORMAL_MAP,
	SPECULAR_MAP
};

class Texture : public BaseAsset
{
private:
	std::string filename;
	unsigned int textureId;
	TextureType textureType;
public:
	Texture();
	virtual ~Texture();
	Texture(const std::string& filename, TextureType type);
	
	bool load();
	unsigned int getTextureId();
	TextureType getTextureType();
	void setTextureType(TextureType type);
	bool load(const std::string& filename);
};