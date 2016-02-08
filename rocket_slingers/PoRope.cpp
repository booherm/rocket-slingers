#include "PoRope.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "Utilities.hpp"

PoRope::PoRope(GameState* gameState) : PhysicalObject(gameState) {
	initGeometry();
	initShaders();

	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
}

void PoRope::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoRope callback.  InputEvent = " << std::endl;

	float worldX = ((float)inputEvent.xCoordinate * gameState->aspectRatio) / gameState->resolutionWidth;
	float worldY = 1.0f - ((float)inputEvent.yCoordinate / gameState->resolutionHeight);
	std::cout << "world space coords (" << worldX << ", " << worldY << ")" << std::endl;

	if (ropeCount < maxRopeCount) {
		ropeOriginPoints.push_back(glm::vec2(0.25f, 0.1f));  // debug - temp guy position
		ropeTerminationPoints.push_back(glm::vec2(worldX, worldY));
		ropeCount++;
	}

	inputEvent.print();
}

void PoRope::initGeometry() {

	// init buffers and vertex array object
	colorSize = maxRopeCount * ropeSegmentCount;
	colorData.resize(colorSize);
	transformSize = maxRopeCount * ropeSegmentCount;
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);

	// buffer axes model data
	GLfloat ropeModelVertices[] = {1.0f, 0.0f};
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ropeModelVertices), ropeModelVertices, GL_STATIC_DRAW);

	// initialize rope color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colorSize, NULL, GL_STREAM_DRAW);

	// initialize guy transform buffer
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

void PoRope::initShaders() {
	// vertex shader
	std::string vertexShaderSource =
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
		"    gl_Position = transformMatrix * vec4(position, 0.0f, 1.0f);\n"
		"    fragShaderColor = inColor;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
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
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();
}

void PoRope::updatePhysicalState() {

	if (ropeCount == 0)
		return;

	transformSize = 0;
	colorSize = 0;
	float ropeColorGrad = 0.75f / ropeSegmentCount;


	for (unsigned int i = 0; i < ropeCount; i++) {

		glm::vec2 ropeOriginPoint = ropeOriginPoints[i];
		glm::vec2 ropeTerminationPoint = ropeTerminationPoints[i];
		float distance = glm::distance(ropeOriginPoint, ropeTerminationPoint);
		float ropeSegmentLength = distance / ropeSegmentCount;
		glm::vec2 ropeTerminationPointTranslated = ropeTerminationPoint - ropeOriginPoint;
		float theta = Utilities::angleBetweenVectors(glm::vec2(1.0f, 0.0f), ropeTerminationPointTranslated);
		glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotationQuaternionMatrix = glm::toMat4(rotationQuaternion);

		for (unsigned int segment = 0; segment < ropeSegmentCount; segment++) {
			// color
			glm::vec4 ropeColorVector(0.25f + (segment * ropeColorGrad), 0.25f + (segment * ropeColorGrad), 0.25f + (segment * ropeColorGrad), 1.0f);
			colorData[colorSize++] = ropeColorVector;

			// model
			glm::mat4 modelTransform;
			float offsetX = glm::cos(theta) * ropeSegmentLength * segment;
			float offsetY = glm::sin(theta) * ropeSegmentLength * segment;
			glm::vec3 translator(ropeOriginPoint.x + offsetX, ropeOriginPoint.y + offsetY, 0.0f);
			modelTransform = glm::translate(modelTransform, translator);
			modelTransform = glm::scale(modelTransform, glm::vec3(ropeSegmentLength, ropeSegmentLength, 1.0f));
			modelTransform = modelTransform * rotationQuaternionMatrix;

			// view
			glm::mat4 viewTransform = gameState->camera->getViewTransform();

			// projection
			glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

			// combine transform
			glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
			transformData[transformSize++] = transform;
		}
	}

	// buffer color data
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * colorSize, colorData.data());

	// buffer transform data
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * transformSize, transformData.data());
}

void PoRope::render() {
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_LINES, 0, 2, ropeSegmentCount * ropeCount);  // 2 vertices per rope segment, draw total count of rope segments
}

PoRope::~PoRope() {
	glDeleteBuffers(1, &modelVbo);
	glDeleteBuffers(1, &transformVbo);
	glDeleteVertexArrays(1, &masterVao);
}
