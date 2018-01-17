#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader
{
public:

	Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	void Use();
	void CheckCurrentProgram();

	// uniform utility functions
	void SetUniformBool(const std::string &name, bool value);
	void SetUniformInt(const std::string &name, int value);
	void SetUniformFloat(const std::string &name, float value);
	void SetUniformMatrix4(const std::string &name, bool bNeedTranspose, float* value);

	unsigned int shaderProgramID;
};

#endif
