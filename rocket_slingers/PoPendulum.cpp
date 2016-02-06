#include "PoPendulum.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject(gameState) {
	xInc = 1.0f / pendulumCount;
	yInc = 1.0f / pendulumCount;

	initGeometry();
	initShaders();

	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
}

void PoPendulum::inputEventCallback(InputEvent inputEvent){
	std::cout << "this is the call back! inputEvent = " << std::endl;
	inputEvent.print();
	clickCount++;
}

void PoPendulum::initGeometry() {

	// init buffers and vertex array object
	colorSize = pendulumCount;
	colorData.resize(colorSize);
	transformSize = pendulumCount;
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
		"    gl_Position = transformMatrix * vec4(position, 0.0f, 1.0f);\n"
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
	transformSize = 0;
	colorSize = 0;

	glm::vec4 pendulumColorVector;
	if(clickCount % 2 == 0){
		pendulumColorVector = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
		pendulumColorVector = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	// transform pendulums
	GLfloat transY = -0.25f;
	for (unsigned int i = 0; i < pendulumCount; i++) {

		GLfloat transX = (i * (xInc)) - 0.25f;

		// color
		colorData[colorSize++] = pendulumColorVector;

		/*	// translate logical cell location to world space
		unsigned int tailId = scp->sourceSiteId;
		unsigned int tailCol = tailId % cols;
		unsigned int tailRow = rows - (tailId / cols) - 1; // logical location row (x) is top down, but the GL window row (x) is bottom up
		glm::vec2 tail((((tailCol * xInc) + (xInc / 2.0f)) * 2.0f) - 1.0f, (((tailRow * yInc) + (yInc / 2.0f)) * 2.0f) - 1.0f);
		unsigned int headId = scp->destinationSiteId;
		unsigned int headCol = headId % cols;
		unsigned int headRow = rows - (headId / cols) - 1; // logical location row (x) is top down, but the GL window row (x) is bottom up
		glm::vec2 head((((headCol * xInc) + (xInc / 2.0f)) * 2.0f) - 1.0f, (((headRow * yInc) + (yInc / 2.0f)) * 2.0f) - 1.0f);
		*/
		glm::mat4 model;


		// translate
		model = glm::translate(model, glm::vec3(transX, transY, 0.0f));

		// rotate
		//GLfloat theta = angleBetweenVectors(glm::vec2(1.0f, 0.0f), glm::vec2(head.x - tail.x, head.y - tail.y));
		//transform = glm::rotate(transform, glm::quarter_pi<float>() * ((float) i / pendulumCount) * (float) currentTime * 25.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, ((float)i / pendulumCount) * (float)glm::sin(gameState->frameTimeStart) * 25.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, ((float)i / pendulumCount) * (float)glm::sin(gameState->frameTimeStart) * 25.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		// scale
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));


		// view
		glm::mat4 view = gameState->camera->GetViewMatrix();
		//		view = glm::translate(view, glm::vec3(-0.5f, -0.5f, -500.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(0.45f, 1920.0f / 1280.0f, 0.1f, 100.0f);
		glm::mat4 transform = projection * view * model;

		//glm::mat4 ortho;
		//		ortho = glm::ortho(-1920.0f / 1280.0f, 1920.0f / 1280.0f, 1920.0f, 0.0f, 1280.0f);
		//		glm::mat4 transform = ortho * view * model;


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
