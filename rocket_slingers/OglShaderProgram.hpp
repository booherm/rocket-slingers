#ifndef OGLSHADERPROGRAM_HPP
#define OGLSHADERPROGRAM_HPP

#include <glew.h>
#include <string>

class OglShaderProgram
{
public:
	OglShaderProgram();
	void createVertexShaderFromSourceString(const std::string& shaderSource);
	void createFragmentShaderFromSourceString(const std::string& shaderSource);
	void createVertexShaderFromFile(const std::string& fileName);
	void createFragmentShaderFromFile(const std::string& fileName);
	void build();
	void use();
	GLuint getProgramId();

private:
	bool built;
	bool vertexShaderSet;
	bool fragmentShaderSet;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	void readFile(const std::string& fileName, std::string& sourceCode);
	GLuint buildShader(GLuint shaderType, const std::string& shaderSource);
	GLuint programId;
};

#endif
