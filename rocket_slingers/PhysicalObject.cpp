#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(const std::string& objectType, GameState* gameState) {
	this->objectType = objectType;
	this->gameState = gameState;

	shouldRender = false;
	glRenderingMode = GL_TRIANGLES;

	shouldDoPhysicalUpdate = false;
	changeInTime = 0.0f;
	maxAllowedChangeInTime = 1.0f;
}

PhysicalObject::~PhysicalObject() {}

void PhysicalObject::updateRenderState() {
	if (shouldRender)
		doRenderUpdate();
}

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

std::vector<glm::vec2>* PhysicalObject::getTextureCoordinateData() {
	return &textureCoordinateData;
}

std::vector<unsigned int>* PhysicalObject::getTextures() {
	return &textures;
}

OglShaderProgram* PhysicalObject::getShaderProgram() {
	return &shaderProg;
}

void PhysicalObject::updatePhysicalState() {

	if (shouldDoPhysicalUpdate) {
		// Calculate the number of physics iterations required.  Change in time must be sufficiently
		// small enough not to cause excessive accelerations.  Each physical object can specify its
		// own maxAllowedChangeInTime value.
		changeInTime = gameState->fLastFrameTotalTimeSeconds;
		unsigned int physicsUpdateIterationsRequired = (unsigned int)(changeInTime / maxAllowedChangeInTime) + 1;
		if (physicsUpdateIterationsRequired != 0) {
			changeInTime = changeInTime / physicsUpdateIterationsRequired;
		}

		for (unsigned int i = 0; i < physicsUpdateIterationsRequired; ++i) {
			doPhysicalUpdate();
		}
	}

}

void PhysicalObject::sdlInputEventCallback(const Event& eventObj) {}

void PhysicalObject::gameEventCallback(const Event& eventObj) {}

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
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();
}

void PhysicalObject::resetForces() {

	for (auto &mass : componentMasses) {
		mass.force = glm::vec3();
	}

}

PhysicalMass* PhysicalObject::getMainComponentMass() {
	return mainComponentMass;
}

void PhysicalObject::doPhysicalUpdate() {}
void PhysicalObject::doRenderUpdate() {}