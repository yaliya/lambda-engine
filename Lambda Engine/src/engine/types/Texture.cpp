#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const std::string & filename, TextureType type = TextureType::DIFFUSE_MAP)
{
	this->filename = filename;
	this->textureType = type;
}

Texture::~Texture()
{
}

bool Texture::load()
{
	glGenTextures(1, &textureId);
	
	int width, height, channels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		GLenum format = channels == 1 ? GL_RED	:
					    channels == 3 ? GL_RGB	: 
						channels == 4 ? GL_RGBA : GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		return true;
	}

	stbi_image_free(data);

	return false;
}

bool Texture::load(const std::string & filename)
{
	this->filename = filename;
	return this->load();
}

unsigned int Texture::getTextureId()
{
	return textureId;
}

TextureType Texture::getTextureType()
{
	return textureType;
}

void Texture::setTextureType(TextureType type)
{
	this->textureType = type;
}
