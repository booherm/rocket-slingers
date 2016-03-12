#include "RenderableObject.hpp"
#include "SOIL.h"
#include <gtc/type_ptr.hpp>

RenderableObject::RenderableObject(const std::string& id)
{
	this->id = id;
	shouldRender = false;
	zDepth = 0.0f;
	glRenderingMode = GL_TRIANGLES;
}

void RenderableObject::initModelVertexBuffer() {

	glGenBuffers(1, &modelVertexVbo);
	generatedBufferIds.push_back(modelVertexVbo);
	glBindBuffer(GL_ARRAY_BUFFER, modelVertexVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * modelVertexData.size(), modelVertexData.data(), GL_STATIC_DRAW);

	abortOnOpenGlError();
}

void RenderableObject::initModelVertexBuffer(unsigned int reservedSize) {

	glGenBuffers(1, &modelVertexVbo);
	generatedBufferIds.push_back(modelVertexVbo);
	glBindBuffer(GL_ARRAY_BUFFER, modelVertexVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * reservedSize, NULL, GL_STATIC_DRAW);

	abortOnOpenGlError();
}

void RenderableObject::initColorBuffer() {

	glGenBuffers(1, &colorVbo);
	generatedBufferIds.push_back(colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colorData.size(), colorData.data(), GL_STATIC_DRAW);

	abortOnOpenGlError();
}

void RenderableObject::initColorBuffer(unsigned int reservedSize) {

	glGenBuffers(1, &colorVbo);
	generatedBufferIds.push_back(colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * reservedSize, NULL, GL_STATIC_DRAW);

	abortOnOpenGlError();
}

void RenderableObject::initTextureCoordinateBuffer() {

	glGenBuffers(1, &textureCoordinateVbo);
	generatedBufferIds.push_back(textureCoordinateVbo);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textureCoordinateData.size(), textureCoordinateData.data(), GL_STATIC_DRAW);

	abortOnOpenGlError();
}

/*
void RenderableObject::initTransformBuffer() {

	glGenBuffers(1, &transformVbo);
	generatedBufferIds.push_back(transformVbo);
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformData.size(), transformData.data(), GL_STATIC_DRAW);

	abortOnOpenGlError();
}
*/

void RenderableObject::refreshModelVertexBuffer() {

	glBindBuffer(GL_ARRAY_BUFFER, modelVertexVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * modelVertexData.size(), modelVertexData.data());
	abortOnOpenGlError();

}

void RenderableObject::refreshColorBuffer() {

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * colorData.size(), colorData.data());
	abortOnOpenGlError();

}

void RenderableObject::refreshTextureCoordinateBuffer() {

	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * textureCoordinateData.size(), textureCoordinateData.data());
	abortOnOpenGlError();

}

/*
void RenderableObject::refreshTransformBuffer() {

	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * transformData.size(), transformData.data());
	abortOnOpenGlError();

}
*/

void RenderableObject::buildVao(unsigned int vaoComponents) {

	unsigned int vertexAttribIndex = 0;
	glGenVertexArrays(1, &masterVao);
	generatedVaoIds.push_back(masterVao);
	glBindVertexArray(masterVao);

	if (vaoComponents & MODEL_VERTEX) {
		glBindBuffer(GL_ARRAY_BUFFER, modelVertexVbo);
		glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*) 0);
		glEnableVertexAttribArray(vertexAttribIndex++);
		abortOnOpenGlError();
	}
	
	if (vaoComponents & COLOR) {
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*) 0);
		glEnableVertexAttribArray(vertexAttribIndex++);
		abortOnOpenGlError();
	}

	if (vaoComponents & TEXTURE_COORDINATE) {
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateVbo);
		glVertexAttribPointer(vertexAttribIndex, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*) 0);
		glEnableVertexAttribArray(vertexAttribIndex++);
		abortOnOpenGlError();
	}

	/*
	if (vaoComponents & TRANSFORM) {
		glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
		for (unsigned int i = 0; i < 4; i++) {
			glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*) (i * sizeof(glm::vec4)));
			glEnableVertexAttribArray(vertexAttribIndex++);
		}
		abortOnOpenGlError();
	}
	*/

	glBindVertexArray(0);
	abortOnOpenGlError();

}

void RenderableObject::setUniformValue(const std::string& uniformId, const glm::mat4& mat4) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProg.getProgramId(), uniformId.c_str()), 1, false, glm::value_ptr(mat4));
}

void RenderableObject::setUniformValue(const std::string& uniformId, const glm::vec3& vec3) {
	glUniform3fv(glGetUniformLocation(shaderProg.getProgramId(), uniformId.c_str()), 1, glm::value_ptr(vec3));
}

void RenderableObject::generateTexture(const std::string& fileName) {

	unsigned int textureId;
	glGenTextures(1, &textureId);
	textureIds.push_back(textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	abortOnOpenGlError();
}

void RenderableObject::abortOnOpenGlError() {
	unsigned int openGlErrorCode = glGetError();
	if (openGlErrorCode != GL_NO_ERROR) {
		std::cout << "opengl error: " << openGlErrorCode << std::endl;
		throw "RenderableObject::abortOnOpenGlError - OpenGL error: " + openGlErrorCode;
	}
}

RenderableObject::~RenderableObject() {
	for (unsigned int i = 0; i < textureIds.size(); ++i) {
		glDeleteTextures(1, &textureIds[i]);
	}

	for (unsigned int i = 0; i < generatedBufferIds.size(); ++i) {
		glDeleteBuffers(1, &generatedBufferIds[i]);
	}

	for (unsigned int i = 0; i < generatedVaoIds.size(); ++i) {
		glDeleteVertexArrays(1, &generatedVaoIds[i]);
	}

	abortOnOpenGlError();
}
