#include "PoReticule.hpp"

PoReticule::PoReticule(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::MOVE_RETICULE, this);
}

void PoReticule::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"\n"
		"out vec4 fragColor;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
		"    fragColor = colorValue;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragColor;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = fragColor;\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoReticule::gameEventCallback(const Event& eventObj) {
	worldPosition = glm::vec3(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);
}

void PoReticule::initGeometry() {

	glRenderingMode = GL_LINES;

	modelVertexData.push_back(glm::vec3(-0.5f, 0.0f, 0.0f));
	modelVertexData.push_back(glm::vec3(0.5f, 0.0f, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f, -0.5f, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f, 0.5f, 0.0f));

	for (unsigned int i = 0; i < modelVertexData.size(); i++)
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	zDepth = 0.9f;
	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	shouldRender = true;
	gameState->masterRenderer->addRenderableObject(this);
}

void PoReticule::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, worldPosition);
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}
