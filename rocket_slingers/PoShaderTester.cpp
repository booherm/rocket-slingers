#include "PoShaderTester.hpp"

PoShaderTester::PoShaderTester(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
}

void PoShaderTester::initGeometry() {

	zDepth = -1.0f;
	sizeScaler = 1.0f;
	float xScaler = sizeScaler * gameState->worldViewportWidth;
	float yScaler = sizeScaler * gameState->worldViewportHeight;

	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);

	for (unsigned int i = 0; i < modelVertexData.size(); ++i) {
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	initModelVertexBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	shouldRender = true;

	projectionTransform = gameState->camera->getProjectionTransform();

	gameState->masterRenderer->addRenderableObject(this);
}

void PoShaderTester::initShaders() {

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromFile("resources/PoShaderTest.vs");
	shaderProg.createFragmentShaderFromFile("resources/PoShaderTest.fs");
	shaderProg.build();

}

void PoShaderTester::render() {

	shaderProg.use();
	glBindVertexArray(masterVao);

	setUniformValue("transformMatrix", projectionTransform);
	setUniformValue("time", gameState->fFrameTimeStart);
	setUniformValue("resolution", glm::vec2(gameState->resolutionWidth, gameState->resolutionHeight));
	setUniformValue("surfacePosition", glm::vec2(0.0f, 0.0f));

	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();

}
