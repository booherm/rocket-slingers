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
	void build();
	void use();

private:
	bool built;
	bool vertexShaderSet;
	bool fragmentShaderSet;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	GLuint buildShader(GLuint shaderType, const std::string& shaderSource);
	GLuint programId;
	GLuint getProgramId();
};

#endif
