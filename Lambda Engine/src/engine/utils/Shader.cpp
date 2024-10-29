#include "Shader.h"

Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderCode = vertexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		ASSERT(false, "ERROR::SHADER::FILE_NOT_READ");
	}

	int success;
	char infoLog[512];

	const char* vertexShaderSource = vertexShaderCode.c_str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	glGetProgramInfoLog(program, 512, NULL, infoLog);
	ASSERT(success, "ERROR::SHADER::VERTEX::COMPILATION_FAILED");

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	ASSERT(success, "ERROR::SHADER::VERTEX::COMPILATION_FAILED");

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	ASSERT(success, "ERROR::SHADER::PROGRAM::LINKING_FAILED");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
}

void Shader::bind()
{
	glUseProgram(program);
}

GLuint Shader::getProgram()
{
	return program;
}

void Shader::setInt(const std::string & uniform, int value) const
{
	glUniform1i(glGetUniformLocation(program, uniform.c_str()), value);
}

void Shader::setBool(const std::string & uniform, bool value) const
{
	glUniform1i(glGetUniformLocation(program, uniform.c_str()), (int)value);
}

void Shader::setFloat(const std::string & uniform, float value) const
{
	glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
}
