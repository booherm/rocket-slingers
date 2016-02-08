#include "PoPendulum.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject(gameState) {
	initGeometry();
	initShaders();
	pendulumCount = 0;

	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_2, InputEvent::IEKS_PRESS, this);
}

void PoPendulum::inputEventCallback(InputEvent inputEvent){
	std::cout << "this is the call back! inputEvent = " << std::endl;
	inputEvent.print();

	float worldX = ((float) inputEvent.xCoordinate * gameState->aspectRatio) / gameState->resolutionWidth;
	float worldY = 1.0f - ((float) inputEvent.yCoordinate / gameState->resolutionHeight);
	std::cout << "world space coords (" << worldX << ", " << worldY << ")" << std::endl;

	if (pendulumCount < maxPendulumCount) {
		pendulumOrigins.push_back(glm::vec3(worldX, worldY, 0.0f));
		pendulumInitialTimes.push_back((float) gameState->frameTimeStart);
		pendulumCount++;
	}

	clickCount++;
}

void PoPendulum::initGeometry() {

	// init buffers and vertex array object
	colorSize = maxPendulumCount;
	colorData.resize(colorSize);
	transformSize = maxPendulumCount;
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);

	// buffer pendulum model data
	GLfloat pendulumModelVertices[] = {
		0.0f, 0.0f,
		0.6f, 0.0f,
		0.6f, 0.2f,
		0.0f, 0.0f,
		0.6f, 0.2f,
		0.0f, 0.2f,
		0.26666666f, 0.2f,
		0.33333333f, 0.2f,
		0.33333333f, 1.0f,
		0.26666666f, 0.2f,
		0.33333333f, 1.0f,
		0.26666666f, 1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pendulumModelVertices), pendulumModelVertices, GL_STATIC_DRAW);

	// initialize pendulum color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colorSize, NULL, GL_STREAM_DRAW);

	// initialize pendulum transform buffer
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformSize, NULL, GL_STREAM_DRAW);

	// start vertex array object setup
	glBindVertexArray(masterVao);

	// define position attribute (model)
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// define color attribute (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	// define transform attribute (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	for (unsigned int i = 2; i <= 5; i++) {
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)((i - 2) * sizeof(glm::vec4)));
		glVertexAttribDivisor(i, 1);
		glEnableVertexAttribArray(i);
	}

	// end vertex array object setup
	glBindVertexArray(0);
}

void PoPendulum::initShaders() {
	// pendulum vertex shader
	std::string pendulumVertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec2 position;\n"
		"layout (location = 1) in vec4 inColor;\n"
		"layout (location = 2) in mat4 transformMatrix;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position.x + -0.3f, position.y + -1.0f, 0.0f, 1.0f);\n" // applying origin shift here
		"    fragShaderColor = inColor;\n"
		"}\n";

	// pendulum fragment shader
	std::string pendulumFragmentShaderSource =
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
	shaderProg.createVertexShaderFromSourceString(pendulumVertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(pendulumFragmentShaderSource);
	shaderProg.build();
}

void PoPendulum::updatePhysicalState() {

	if (pendulumCount == 0)
		return;

	transformSize = 0;
	colorSize = 0;

	glm::vec4 pendulumColorVector;
	if(clickCount % 2 == 0){
		pendulumColorVector = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
		pendulumColorVector = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	// transform pendulums
	GLfloat scalingFactor = 0.25f;
	for (unsigned int i = 0; i < pendulumCount; i++) {

		// color
		colorData[colorSize++] = pendulumColorVector; 

		// model
		glm::mat4 modelTransform;

		glm::vec3 initialPosition = pendulumOrigins[i];
		float elapsedTime = (float)gameState->frameTimeStart - pendulumInitialTimes[i];
		/*
		// distance travelled due to gravity
		float distance = initialPosition.y - ((0.5f * (9.8f * elapsedTime * elapsedTime)) / 20.0f);
		if (distance < 0.1f)
			distance = 0.1f;
			modelTransform = glm::translate(modelTransform, glm::vec3(initialPosition.x, distance, initialPosition.z));
		*/

		modelTransform = glm::translate(modelTransform, initialPosition);
		modelTransform = glm::scale(modelTransform, glm::vec3(scalingFactor, scalingFactor, 1.0f));
		float theta = glm::cos( glm::sqrt(9.8f) * elapsedTime) * (1.0f / elapsedTime);
		//glm::quat rotationQuaternion = glm::angleAxis(glm::sin((float) gameState->frameTimeStart), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTransform = modelTransform * glm::toMat4(rotationQuaternion);
 
		// view
		glm::mat4 viewTransform = gameState->camera->getViewTransform();
		
		// projection
		glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

		// combine transform
		glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
		transformData[transformSize++] = transform;
	}

	// buffer pendulum color data
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * colorSize, colorData.data());

	// buffer pendulum transform data
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * transformSize, transformData.data());
}

void PoPendulum::render() {
	// draw pendulum states
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 12, pendulumCount);
}

PoPendulum::~PoPendulum() {
	glDeleteBuffers(1, &modelVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &transformVbo);
	glDeleteVertexArrays(1, &masterVao);
}
