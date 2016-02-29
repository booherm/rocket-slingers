#include "PoPhysicsRenderer.hpp"

PoPhysicsRenderer::PoPhysicsRenderer(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
	gameState->physicsManager->setDebugRenderer(this);
}

void PoPhysicsRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {

	glm::vec3 fromVec(from.x(), from.y(), 0.0f);
	glm::vec3 toVec(to.x(), to.y(), 0.0f);

	glm::vec4 colorVec;
	if(color.x() == 0.0f && color.y() == 0.0f  && color.z() == 0.0f)
		colorVec = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		colorVec = glm::vec4(color.x(), color.y(), color.z(), 1.0f);

	modelVertexData.push_back(fromVec);
	modelVertexData.push_back(toVec);
	colorData.push_back(colorVec);
	colorData.push_back(colorVec);
}

void PoPhysicsRenderer::flushLines() {
}

void PoPhysicsRenderer::initShaders() {

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

void PoPhysicsRenderer::initGeometry() {

	glRenderingMode = GL_LINES;
	zDepth = 1.0f;

	unsigned int maxVertices = 10000;
	initModelVertexBuffer(maxVertices);
	initColorBuffer(maxVertices);

	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;

}

PoPhysicsRenderer::~PoPhysicsRenderer() {};

void PoPhysicsRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
};

void PoPhysicsRenderer::reportErrorWarning(const char* warningString) {
	throw std::string(warningString);
};

void PoPhysicsRenderer::draw3dText(const btVector3& location, const char* textString) {
};

void PoPhysicsRenderer::setDebugMode(int debugMode) {
	this->debugMode = debugMode;
};

int	 PoPhysicsRenderer::getDebugMode() const {
	return debugMode;
};

void PoPhysicsRenderer::render() {

	refreshModelVertexBuffer();
	refreshColorBuffer();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	modelVertexData.clear();
	colorData.clear();

	abortOnOpenGlError();
}
