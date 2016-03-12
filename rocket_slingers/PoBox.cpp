#include "PoBox.hpp"

PoBox::PoBox(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
	initPhysics();
}

void PoBox::initShaders() {

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

void PoBox::initGeometry() {

	position = glm::vec3(10.0f, 10.0f, 0.0f);


	modelVertexData.push_back(glm::vec3(-1.0f * xScaler, -1.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, -1.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(-1.0f * xScaler, -1.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(-1.0f * xScaler, 1.0f * yScaler, 0.0f));

	// push color data
	for (unsigned int i = 0; i < modelVertexData.size(); i++) {
		colorData.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	}

	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoBox::initPhysics() {

	glm::mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, position);

	physicalMass = new PhysicalMass();
	physicalMass->init("PO_BOX", gameState, 0.0f, worldTransform, PhysicsManager::CollisionGroup::BOUNDARY);
	physicalMass->addCollisionShapeBox(glm::mat4(), glm::vec3(2.0f * xScaler, 2.0f * yScaler, 0.0f));
	physicalMass->addToDynamicsWorld();

	shouldDoPhysicalUpdate = true;

}

void PoBox::doPhysicalUpdate() {
	physicalMass->getCenterOfMassPosition(position);
}

void PoBox::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, position);
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}


PoBox::~PoBox() {
	delete physicalMass;
}
