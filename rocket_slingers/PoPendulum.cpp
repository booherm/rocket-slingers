#include "PoPendulum.hpp"

PoPendulum::PoPendulum(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initialPosition = glm::vec3(15.0f, 15.0f, 0.0f);
	initShaders();
	initGeometry();
	initPhysics();
}

void PoPendulum::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"\n"
		"out vec4 fragColor;\n"
		"\n"
		"uniform vec3 modelOriginOffset;\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex + modelOriginOffset, 1.0f);\n"
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

void PoPendulum::initGeometry() {

	// pendulum model points
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(-0.5f, -0.15f, 0.0f)); // A = 0
	points.push_back(glm::vec3(0.5f, -0.15f, 0.0f));  // B = 1
	points.push_back(glm::vec3(0.5f, 0.15f, 0.0f));   // C = 2
	points.push_back(glm::vec3(0.05f, 0.15f, 0.0f));  // D = 3
	points.push_back(glm::vec3(0.05f, 0.85f, 0.0f));  // E = 4
	points.push_back(glm::vec3(-0.05f, 0.85f, 0.0f)); // F = 5
	points.push_back(glm::vec3(-0.05f, 0.15f, 0.0f)); // G = 6
	points.push_back(glm::vec3(-0.5f, 0.15f, 0.0f));  // H = 7
	points.push_back(glm::vec3(0.0f, -0.15f, 0.0f));  // I = 8

													  // pendulum model triangles
	modelVertexData.push_back(points[0]);
	modelVertexData.push_back(points[6]);
	modelVertexData.push_back(points[7]);
	modelVertexData.push_back(points[0]);
	modelVertexData.push_back(points[8]);
	modelVertexData.push_back(points[6]);
	modelVertexData.push_back(points[8]);
	modelVertexData.push_back(points[3]);
	modelVertexData.push_back(points[6]);
	modelVertexData.push_back(points[8]);
	modelVertexData.push_back(points[1]);
	modelVertexData.push_back(points[3]);
	modelVertexData.push_back(points[1]);
	modelVertexData.push_back(points[2]);
	modelVertexData.push_back(points[3]);
	modelVertexData.push_back(points[6]);
	modelVertexData.push_back(points[3]);
	modelVertexData.push_back(points[4]);
	modelVertexData.push_back(points[6]);
	modelVertexData.push_back(points[4]);
	modelVertexData.push_back(points[5]);

	modelOriginOffset = glm::vec3(0.0f, -0.85f, 0.0f);

	// color
	glm::vec4 modelColor(1.0f, 1.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < modelVertexData.size(); i++)
		colorData.push_back(modelColor);

	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoPendulum::initPhysics() {

	// rigid body
	b2BodyDef rigidBodyDef;
	rigidBodyDef.type = b2_dynamicBody;
	rigidBodyDef.position.Set(initialPosition.x, initialPosition.y);
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);
	b2PolygonShape rigidBodyShape;
	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.density = 1.0f;
	rigidBodyFixtureDef.friction = 0.3f;
	rigidBodyFixtureDef.userData = this;

	// bottom fixture
	rigidBodyShape.SetAsBox(0.5f * sizeScaler, 0.15f * sizeScaler, b2Vec2(0.0f, -0.85f * sizeScaler), 0.0f);
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	rigidBody->CreateFixture(&rigidBodyFixtureDef);

	// arm fixture
	rigidBodyShape.SetAsBox(0.05f * sizeScaler, 0.35f * sizeScaler, b2Vec2(0.0f, -0.35f * sizeScaler), 0.0f);
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	rigidBody->CreateFixture(&rigidBodyFixtureDef);

	// revolute joint
	b2RevoluteJointDef revJointDef;
	revJointDef.bodyA = gameState->physicsManager->worldStaticBody;
	revJointDef.bodyB = rigidBody;
	revJointDef.localAnchorA = b2Vec2(initialPosition.x, initialPosition.y);
	revJointDef.localAnchorB = b2Vec2(0.0f, 0.0f);
	revJointDef.collideConnected = false;
	revJoint = (b2RevoluteJoint*) gameState->physicsManager->box2dWorld->CreateJoint(&revJointDef);

	shouldDoPhysicalUpdate = true;

}

void PoPendulum::doPhysicalUpdate() {

	/*
	glm::vec3 pos;
	bobMass->getCenterOfMassPosition(pos);

	Event e;
	e.eventPoster = this;
	e.eventType = Event::EventType::GAME_EVENT;
	e.gameEvent = Event::GameEvent::MOVE_RETICULE;
	e.eventWorldCoordinateX = pos.x;
	e.eventWorldCoordinateY = pos.y;
	gameState->eventBus->postEvent(e);
	*/
}

void PoPendulum::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, initialPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(sizeScaler, sizeScaler, 1.0f));
	glm::quat rotationQuaternion;
	rotationQuaternion = glm::rotate(rotationQuaternion, rigidBody->GetAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	setUniformValue("modelOriginOffset", modelOriginOffset);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}

PoPendulum::~PoPendulum() {
	gameState->physicsManager->box2dWorld->DestroyJoint(revJoint);
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);
}
