#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(const std::string& objectType, GameState* gameState) {
	this->objectType = objectType;
	this->gameState = gameState;
	mass = 0.0f;
	shouldRender = true;
	glRenderingMode = GL_TRIANGLES;
}

PhysicalObject::~PhysicalObject() {}

std::vector<glm::vec3>* PhysicalObject::getModelVertices() {
	return &modelVertices;
}

std::vector<glm::vec3>* PhysicalObject::getModelOriginOffsetData() {
	return &modelOriginOffsetData;
}

std::vector<glm::vec4>* PhysicalObject::getColorData() {
	return &colorData;
}

std::vector<glm::mat4>* PhysicalObject::getTransformData() {
	return &transformData;
}

OglShaderProgram* PhysicalObject::getShaderProgram() {
	return &shaderProg;
}

void PhysicalObject::updatePhysicalState() {}
void PhysicalObject::inputEventCallback(InputEvent inputEvent) {}

void PhysicalObject::initGeometry() {}

void PhysicalObject::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 modelOriginOffset;\n"
		"layout (location = 2) in vec4 inColor;\n"
		"layout (location = 3) in mat4 transformMatrix;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position + modelOriginOffset, 1.0f);\n"
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

void PhysicalObject::resetForce() {
	force.x = 0.0f;
	force.y = 0.0f;
	force.z = 0.0f;
}

void PhysicalObject::applyForce(glm::vec3 force) {
	this->force += force;
}

void PhysicalObject::applyAcceleration(glm::vec3 acceleration) {
	force += (mass * acceleration);
}

void PhysicalObject::updatePhysics() {
	if (mass != 0.0f) {
		velocity += ((force / mass) * gameState->fLastFrameTotalTime);
		worldPosition += (velocity * gameState->fLastFrameTotalTime);
	}
}
