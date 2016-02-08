#include "PoGuy.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

PoGuy::PoGuy(GameState* gameState) : PhysicalObject(gameState) {
	initGeometry();
	initShaders();

	//inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
}

void PoGuy::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoGuy callback.  InputEvent = " << std::endl;
	inputEvent.print();
}

void PoGuy::initGeometry() {

	// init buffers and vertex array object
	colorSize = guyCount;
	colorData.resize(guyCount);
	guyTriangleCount = 53;
	transformSize = guyTriangleCount;
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);

	// buffer axes model data
	GLfloat guyModelVerticesSet[57][2] = {
		{ 0.0f, 0.0f },
		{ 0.0f, 5.0f },
		{ 2.0f, 4.0f },
		{ 2.5f, 5.0f },
		{ 3.0f, 4.0f },
		{ 6.0f, 4.0f },
		{ 7.0f, 8.0f },
		{ 8.0f, 11.0f },
		{ 8.0f, 15.0f },
		{ 7.0f, 16.0f },
		{ 7.0f, 18.0f },
		{ 7.5f, 20.0f },
		{ 11.0f, 28.0f },
		{ 11.5f, 29.0f },
		{ 11.0f, 31.0f },
		{ 10.0f, 33.0f },
		{ 9.0f, 35.5f },
		{ 10.0f, 36.5f },
		{ 15.0f, 40.0f },
		{ 15.5f, 41.5f },
		{ 15.5f, 43.0f },
		{ 16.0f, 45.0f },
		{ 17.0f, 46.0f },
		{ 18.5f, 46.5f },
		{ 20.0f, 46.0f },
		{ 21.0f, 45.0f },
		{ 21.5f, 43.0f },
		{ 21.0f, 40.0f },
		{ 24.0f, 37.0f },
		{ 26.0f, 33.0f },
		{ 25.5f, 31.0f },
		{ 23.5f, 28.5f },
		{ 26.0f, 20.0f },
		{ 28.0f, 8.0f },
		{ 29.5f, 6.0f },
		{ 30.0f, 4.0f },
		{ 33.0f, 4.0f },
		{ 35.0f, 2.5f },
		{ 33.0f, 1.0f },
		{ 3.0f, 1.0f },
		{ 2.5f, 0.0f },
		{ 2.0f, 1.0f },
		{ 10.5f, 4.0f },
		{ 9.5f, 5.0f },
		{ 9.5f, 8.0f },
		{ 11.5f, 12.0f },
		{ 12.0f, 16.0f },
		{ 14.0f, 20.0f },
		{ 16.0f, 22.0f },
		{ 17.0f, 22.0f },
		{ 18.5f, 19.0f },
		{ 19.0f, 16.0f },
		{ 20.0f, 15.0f },
		{ 21.5f, 14.5f },
		{ 22.5f, 13.5f },
		{ 24.0f, 9.0f },
		{ 24.5f, 4.0f }
	};

	unsigned int guyModelTriangleVertexIds[53][3] = {
		{ 0,41,1 },
		{ 41,2,1 },
		{ 41,4,2 },
		{ 2,4,3 },
		{ 40,39,41 },
		{ 41,39,4 },
		{ 39,36,4 },
		{ 39,38,36 },
		{ 38,37,36 },
		{ 5,42,43 },
		{ 5,43,44 },
		{ 5,44,6 },
		{ 6,44,7 },
		{ 44,45,7 },
		{ 7,45,8 },
		{ 45,46,8 },
		{ 8,46,9 },
		{ 9,46,10 },
		{ 46,47,10 },
		{ 10,47,11 },
		{ 11,47,48 },
		{ 11,48,12 },
		{ 48,49,12 },
		{ 49,32,12 },
		{ 32,31,12 },
		{ 12,31,13 },
		{ 13,31,30 },
		{ 13,30,14 },
		{ 14,30,15 },
		{ 30,29,15 },
		{ 15,29,16 },
		{ 16,29,17 },
		{ 29,28,17 },
		{ 17,28,18 },
		{ 28,27,18 },
		{ 18,27,19 },
		{ 27,26,19 },
		{ 19,26,20 },
		{ 20,26,25 },
		{ 20,25,21 },
		{ 21,25,24 },
		{ 21,24,22 },
		{ 22,24,23 },
		{ 50,32,49 },
		{ 51,32,50 },
		{ 52,32,51 },
		{ 53,32,52 },
		{ 54,32,53 },
		{ 33,32,54 },
		{ 55,33,54 },
		{ 56,33,55 },
		{ 56,34,33 },
		{ 56,35,34 }
	};

	GLfloat guyModelVertices[53 * 3 * 2]; // 53 triangles, 3 vertices per triangle, 2 coordinate values per vertex
	unsigned int guyModelVertexIndex = 0;
	for (unsigned int t = 0; t < guyTriangleCount; t++) {
		for (unsigned int v = 0; v < 3; v++) {
			guyModelVertices[guyModelVertexIndex++] = guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][0];
			guyModelVertices[guyModelVertexIndex++] = guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][1];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(guyModelVertices), guyModelVertices, GL_STATIC_DRAW);

	// initialize guy color buffer
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

void PoGuy::initShaders() {
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

void PoGuy::updatePhysicalState() {
	transformSize = 0;
	colorSize = 0;

	glm::vec4 guyColorVector(0.0f, 0.0f, 1.0f, 1.0f);

	// transform guy
	for (unsigned int i = 0; i < guyCount; i++) {

		// color
		colorData[colorSize++] = guyColorVector;

		// model transform
		// translate, scale, rotate

		//glm::vec3 initialPosition(0.25f, 0.5f, 0.0f);
		// distance travelled due to gravity
		//float distance = 1.0f - ((0.5f * (9.8f * (float) gameState->frameTimeStart * (float) gameState->frameTimeStart)) / 20.0f);
		//if (distance < 0.05f)
//			distance = 0.05f;
		//glm::vec3 position(0.25f, distance, 0.0f);
		glm::vec3 position(0.25f, 0.1f, 0.0f);


		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, position);
		modelTransform = glm::scale(modelTransform, glm::vec3(0.0015f, 0.0015f, 1.0f));

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

void PoGuy::render() {
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, guyTriangleCount * 3, guyCount);
}

PoGuy::~PoGuy() {
	glDeleteBuffers(1, &modelVbo);
	glDeleteBuffers(1, &transformVbo);
	glDeleteVertexArrays(1, &masterVao);
}
