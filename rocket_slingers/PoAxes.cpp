#include "PoAxes.hpp"

PoAxes::PoAxes(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
}

void PoAxes::initShaders() {

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

void PoAxes::initGeometry() {

	glRenderingMode = GL_LINES;

	float xMinorTickInterval = 1.0f;
	float xMajorTickInterval = 5.0f;
	float yMinorTickInterval = 1.0f;
	float yMajorTickInterval = 5.0f;

	float xHalfExtent = (((unsigned int) (1000.0f * gameState->aspectRatio)) / (unsigned int) xMajorTickInterval) * xMajorTickInterval;
	float yHalfExtent = 1000.0f;

	glm::vec3 color(0.0f, 1.0f, 0.0f);
	float minorTickAlpha = 0.25f;
	float majorTickAlpha = 1.00f;

	glm::vec4 axesColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 minorTickColor = glm::vec4(color, minorTickAlpha);
	glm::vec4 majorTickColor = glm::vec4(color, majorTickAlpha);

	// y axis
	modelVertexData.push_back(glm::vec3(0.0f, -yHalfExtent, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f, yHalfExtent, 0.0f));
	colorData.push_back(axesColor);
	colorData.push_back(axesColor);

	// x minor ticks
	unsigned int minorTickCount = (unsigned int) xHalfExtent / (unsigned int) xMinorTickInterval;
	float tickLocation = xMinorTickInterval;
	for (unsigned int i = 1; i <= minorTickCount; ++i) {
		modelVertexData.push_back(glm::vec3(tickLocation, -yHalfExtent, 0.0f));
		modelVertexData.push_back(glm::vec3(tickLocation, yHalfExtent, 0.0f));
		modelVertexData.push_back(glm::vec3(-tickLocation, -yHalfExtent, 0.0f));
		modelVertexData.push_back(glm::vec3(-tickLocation, yHalfExtent, 0.0f));

		if (i % (unsigned int) xMajorTickInterval == 0) {
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
		}
		else {
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
		}

		tickLocation += xMinorTickInterval;
	}

	// x axis
	modelVertexData.push_back(glm::vec3(-xHalfExtent, 0.0f, 0.0f));
	modelVertexData.push_back(glm::vec3(xHalfExtent, 0.0f, 0.0f));
	colorData.push_back(axesColor);
	colorData.push_back(axesColor);

	// y minor ticks
	minorTickCount = (unsigned int) yHalfExtent / (unsigned int) yMinorTickInterval;
	tickLocation = yMinorTickInterval;
	for (unsigned int i = 1; i <= minorTickCount; ++i) {
		modelVertexData.push_back(glm::vec3(-xHalfExtent, tickLocation, 0.0f));
		modelVertexData.push_back(glm::vec3(xHalfExtent, tickLocation, 0.0f));
		modelVertexData.push_back(glm::vec3(-xHalfExtent, -tickLocation, 0.0f));
		modelVertexData.push_back(glm::vec3(xHalfExtent, -tickLocation, 0.0f));

		if (i % (unsigned int) yMajorTickInterval == 0) {
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
			colorData.push_back(majorTickColor);
		}
		else {
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
			colorData.push_back(minorTickColor);
		}

		tickLocation += yMinorTickInterval;
	}

	zDepth = 0.9f;
	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	shouldRender = true;
	gameState->masterRenderer->addRenderableObject(this);

}

void PoAxes::render() {

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();

}
