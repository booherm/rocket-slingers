#include "PoBall.hpp"

PoBall::PoBall(const std::string& objectId, GameState* gameState, float radius, const b2Vec2& position) : PhysicalObject(objectId, gameState) {

	this->position = position;
	this->radius = radius;
	ropeAttachable = true;
	ropeAttachmentPoint = position;

	initShaders();
	initGeometry();
	initPhysics();
}

void PoBall::initShaders() {

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

void PoBall::initGeometry() {

	unsigned int resolution = 20;
	for (unsigned int i = 0; i < resolution; i++) {

		float x = radius * glm::cos((glm::two_pi<float>() / resolution) * i);
		float y = radius * glm::sin((glm::two_pi<float>() / resolution) * i);
		float nextX = radius * glm::cos((glm::two_pi<float>() / resolution) * (i + 1));
		float nextY = radius * glm::sin((glm::two_pi<float>() / resolution) * (i + 1));

		modelVertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		modelVertexData.push_back(glm::vec3(x, y, 0.0f));
		modelVertexData.push_back(glm::vec3(nextX, nextY, 0.0f));


	}

	// texture and color data
	for (unsigned int i = 0; i < modelVertexData.size(); ++i) {
		textureCoordinateData.push_back(glm::vec2(modelVertexData[i].x * 0.025f, modelVertexData[i].y * 0.025f));
		colorData.push_back(glm::vec4(0.9216f, 0.1059f, 0.9490f, 1.0f));
	}

	std::string fileName = "resources/microscheme.png";
	generateTexture(fileName);
	transformData.push_back(glm::mat4());

	initModelVertexBuffer();
	initColorBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | COLOR | TEXTURE_COORDINATE);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;

}

void PoBall::initPhysics() {

	b2BodyDef rigidBodyDef;
	rigidBodyDef.type = b2_dynamicBody;
	rigidBodyDef.position = position;
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);
	b2CircleShape rigidBodyShape;
	rigidBodyShape.m_radius = radius;
	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	rigidBodyFixtureDef.friction = 0.3f;
	rigidBodyFixtureDef.userData = this;
	rigidBodyFixtureDef.filter.categoryBits = PhysicsManager::CollisionCategory::DRAGABLE_MASS;
	rigidBodyFixtureDef.filter.maskBits = gameState->physicsManager->getCollisionMask(PhysicsManager::CollisionCategory::DRAGABLE_MASS);
	rigidBody->CreateFixture(&rigidBodyFixtureDef);

	shouldDoPhysicalUpdate = true;
}

void PoBall::doPhysicalUpdate() {
	position = rigidBody->GetPosition();
	ropeAttachmentPoint = position;
}


void PoBall::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, Utilities::b2Vec2ToGlmVec3(position));
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


PoBall::~PoBall() {
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);
}
