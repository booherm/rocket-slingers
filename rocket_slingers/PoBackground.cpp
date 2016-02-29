#include "PoBackground.hpp"
#include "SOIL.h"
#include <gtc/type_ptr.hpp>

PoBackground::PoBackground(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
}

void PoBackground::initGeometry() {

	zDepth = -1.0f;
	sizeScaler = 1.0f;
	float xScaler = sizeScaler * gameState->worldViewportScaler * gameState->aspectRatio;
	float yScaler = sizeScaler * gameState->worldViewportScaler;

	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);

	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 1.0f));

	parallaxLayers = 4;
	for (unsigned int i = 0; i < parallaxLayers; ++i) {
		std::string fileName = "resources/backgrounds/background_03_parallax_0" + std::to_string(i + 1) + ".png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
	}

	initModelVertexBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | TEXTURE_COORDINATE);
	shouldRender = true;

	gameState->masterRenderer->addRenderableObject(this);
}

void PoBackground::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec2 textureCoordinate;\n"
		"\n"
		"out vec2 fragTextureCoordinate;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
		"    fragTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y);\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec2 fragTextureCoordinate;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = texture(texture0, fragTextureCoordinate);\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoBackground::render() {
	
//	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	
	shaderProg.use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(masterVao);
	for (unsigned int i = 0; i < parallaxLayers; i++) {

//		glm::mat4 transform = projectionTransform * viewTransform * transformData[i];
		glm::mat4 transform = projectionTransform * transformData[i];
		setUniformValue("transformMatrix", transform);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		glDrawArrays(glRenderingMode, 0, 6);

		transformData[i] = glm::translate(transformData[i], glm::vec3(0.005f * i, 0.0f, 0.0f));

	}

	abortOnOpenGlError();

}
