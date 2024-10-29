#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "Assert.h"
#include "GL\glew.h"

class Shader
{
public:
	unsigned int program;

	Shader(const char* vertexShader, const char* fragmentShader);
	virtual ~Shader();

	void bind();
	GLuint getProgram();
	void setInt(const std::string &uniform, int value) const;
	void setBool(const std::string &uniform, bool value) const;
	void setFloat(const std::string &uniform, float value) const;
};