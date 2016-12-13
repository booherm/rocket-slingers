#include "PoSpinAnchor.hpp"

PoSpinAnchor::PoSpinAnchor(const std::string& objectId, GameState* gameState, const glm::vec3& scalers, const glm::vec3& position) : PhysicalObject(objectId, gameState) {

	this->scalers = scalers;
	this->position = position;
	ropeAttachable = true;
	ropeAttachmentPoint = Utilities::glmVec3ToB2Vec2((position + 0.5f) * scalers);

	initShaders();
	initGeometry();
	initPhysics();
}

void PoSpinAnchor::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"layout (location = 2) in vec2 textureCoordinate;\n"
		"\n"
		"out vec4 fragColor;\n"
		"out vec2 fragTextureCoordinate;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
		"    fragTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y);\n"
		"    fragColor = colorValue;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragColor;\n"
		"in vec2 fragTextureCoordinate;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = texture(texture0, fragTextureCoordinate) * fragColor;\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoSpinAnchor::initGeometry() {

	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 1.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 1.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 1.0f * scalers.y, 0.0f));

	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(0.25f * scalers.x, 0.0f));
	textureCoordinateData.push_back(glm::vec2(0.25f * scalers.x, 0.25f * scalers.y));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(0.25f * scalers.x, 0.25f * scalers.y));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.25f * scalers.y));

	std::string fileName = "resources/microscheme.png";
	generateTexture(fileName);
	transformData.push_back(glm::mat4());

	// push color data
	for (unsigned int i = 0; i < modelVertexData.size(); i++) {
		colorData.push_back(glm::vec4(0.9216f, 0.1059f, 0.9490f, 1.0f));
	}

	initModelVertexBuffer();
	initColorBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | COLOR | TEXTURE_COORDINATE);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoSpinAnchor::initPhysics() {

	b2BodyDef rigidBodyDef;
	rigidBodyDef.position.Set(position.x + (scalers.x / 2.0f), position.y + (scalers.y / 2.0f));
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);
	b2CircleShape rigidBodyShape;
	rigidBodyShape.m_radius = 2.00f;
	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	rigidBodyFixtureDef.friction = 0.3f;
	rigidBodyFixtureDef.userData = this;
	rigidBodyFixtureDef.filter.categoryBits = PhysicsManager::CollisionCategory::ATTACHMENT_POINT;
	rigidBodyFixtureDef.filter.maskBits = gameState->physicsManager->getCollisionMask(PhysicsManager::CollisionCategory::ATTACHMENT_POINT);
	rigidBody->CreateFixture(&rigidBodyFixtureDef);


}

void PoSpinAnchor::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, position);
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(masterVao);
	glBindTexture(GL_TEXTURE_2D, textureIds[0]);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}


PoSpinAnchor::~PoSpinAnchor() {
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);
}
