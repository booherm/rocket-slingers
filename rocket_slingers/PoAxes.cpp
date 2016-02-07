#include "PoAxes.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

PoAxes::PoAxes(GameState* gameState) : PhysicalObject(gameState) {
	initGeometry();
	initShaders();

	//inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
}

void PoAxes::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoAxes callback.  InputEvent = " << std::endl;
	inputEvent.print();
}

void PoAxes::initGeometry() {

	// init buffers and vertex array object
	//axesLinesCount = ((minorTicks + majorTicks) * 3) + 3;
	axesLinesCount = 3;
	transformSize = axesLinesCount;
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &transformVbo);

	// buffer axes model data
	GLfloat axesModelVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axesModelVertices), axesModelVertices, GL_STATIC_DRAW);

	// initialize axes transform buffer
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformSize, NULL, GL_STREAM_DRAW);

	// start vertex array object setup
	glBindVertexArray(masterVao);

	// define position attribute (model)
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// define transform attribute (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	for (unsigned int i = 1; i <= 4; i++) {
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)((i - 1) * sizeof(glm::vec4)));
		glVertexAttribDivisor(i, 1);
		glEnableVertexAttribArray(i);
	}

	// end vertex array object setup
	glBindVertexArray(0);
}

void PoAxes::initShaders() {
	// axes vertex shader
	std::string axesVertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec2 position;\n"
		"layout (location = 1) in mat4 transformMatrix;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position, 0.0f, 1.0f);\n"
		"    fragShaderColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n";

	// axes fragment shader
	std::string axesFragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragShaderColor;\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = fragShaderColor;\n"
		"}\n\0";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(axesVertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(axesFragmentShaderSource);
	shaderProg.build();
}

void PoAxes::updatePhysicalState() {
	transformSize = 0;

	// transform axes
	//modelX = glm::translate(modelX, glm::vec3(xMin, 0.0f, 0.0f));
	//model = glm::rotate(model, ((float)i / axesCount) * (float)glm::sin(currentTime) * 25.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//	modelX = glm::scale(modelX, glm::vec3(xMax - xMin, 1.0f, 1.0f));

	// x axis
	glm::mat4 modelX;
	modelX = glm::translate(modelX, glm::vec3(xMin, 0.0f, 0.0f));
	modelX = glm::rotate(modelX, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelX = glm::scale(modelX, glm::vec3(xMax - xMin, 1.0f, 1.0f));
	pushAxisTransform(modelX);

	// y axis
	glm::mat4 modelY;
	modelY = glm::translate(modelY, glm::vec3(0.0f, yMin, 0.0f));
	modelY = glm::rotate(modelY, glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelY = glm::scale(modelY, glm::vec3(yMax - yMin, 1.0f, 1.0f));
	pushAxisTransform(modelY);

	// z axis
	glm::mat4 modelZ;
	modelZ = glm::translate(modelZ, glm::vec3(0.0f, 0.0f, zMin));
	modelZ = glm::rotate(modelZ, -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	modelZ = glm::scale(modelZ, glm::vec3(zMax - zMin, 1.0f, 1.0f));
	pushAxisTransform(modelZ);

	// buffer axes transform data
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * transformSize, transformData.data());
}

void PoAxes::pushAxisTransform(glm::mat4 model) {
	glm::mat4 view = gameState->camera->getViewTransform();
	glm::mat4 projection = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projection * view * model;
	transformData[transformSize++] = transform;
}

void PoAxes::render() {
	// draw axes states
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_LINES, 0, 2, axesLinesCount);
}

PoAxes::~PoAxes() {
	glDeleteBuffers(1, &modelVbo);
	glDeleteBuffers(1, &transformVbo);
	glDeleteVertexArrays(1, &masterVao);
}
