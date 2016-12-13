#include "PoTubeSection.hpp"

PoTubeSection::PoTubeSection(const std::string& objectId, GameState* gameState, TileType tileType, const b2Vec2& scalers, const b2Vec2& position) : PhysicalObject(objectId, gameState) {

	tileTransforms[STRAIGHT_HORZ] = { 0.0f, glm::vec3() };
	tileTransforms[STRAIGHT_VERT] = { -glm::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f) };
	tileTransforms[LEFT_BOTTOM_90] = { 0.0f, glm::vec3() };
	tileTransforms[LEFT_TOP_90] = { -glm::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f) };
	tileTransforms[TOP_LEFT_90] = { -glm::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f) };
	tileTransforms[TOP_RIGHT_90] = { -glm::pi<float>(), glm::vec3(-1.0f, -1.0f, 0.0f) };
	tileTransforms[RIGHT_TOP_90] = { -glm::pi<float>(), glm::vec3(-1.0f, -1.0f, 0.0f) };
	tileTransforms[RIGHT_BOTTOM_90] = { glm::half_pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f) };
	tileTransforms[BOTTOM_LEFT_90] = { 0.0f, glm::vec3() };
	tileTransforms[BOTTOM_RIGHT_90] = { glm::half_pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f) };

	this->tileType = tileType;
	this->scalers = scalers;
	this->position = position;

	initShaders();
	initGeometry();
	initPhysics();
}

void PoTubeSection::initShaders() {

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

void PoTubeSection::initGeometry() {

	float rotation = 0.0f;

	switch (tileType) {
		case STRAIGHT_HORZ:
		case STRAIGHT_VERT: {

			// bottom
			modelVertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 0.4f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 0.4f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 0.4f, 0.0f));
			lowerFixture.push_back(b2Vec2(0.0f, 0.0f));
			lowerFixture.push_back(b2Vec2(1.0f, 0.0f));
			lowerFixture.push_back(b2Vec2(1.0f, 0.4f));
			lowerFixture.push_back(b2Vec2(0.0f, 0.4f));

			// center
			centerFixture.push_back(b2Vec2(0.0f, 0.4f));
			centerFixture.push_back(b2Vec2(1.0f, 0.4f));
			centerFixture.push_back(b2Vec2(1.0f, 0.6f));
			centerFixture.push_back(b2Vec2(0.0f, 0.6f));

			// top
			modelVertexData.push_back(glm::vec3(0.0f, 0.6f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 0.6f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 0.6f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			upperFixture.push_back(b2Vec2(0.0f, 0.6f));
			upperFixture.push_back(b2Vec2(1.0f, 0.6f));
			upperFixture.push_back(b2Vec2(1.0f, 1.0f));
			upperFixture.push_back(b2Vec2(0.0f, 1.0f));

			// graphics transform
			glm::mat4 transform;
			transform = glm::scale(transform, Utilities::b2Vec2ToGlmVec3(scalers));
			glm::quat rotationQuaternion;
			rotationQuaternion = glm::rotate(rotationQuaternion, tileTransforms[tileType].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			transform = transform * glm::toMat4(rotationQuaternion);
			transform = glm::translate(transform, tileTransforms[tileType].translation);
			for (unsigned int i = 0; i < modelVertexData.size(); ++i) {
				modelVertexData[i] = glm::vec3(transform * glm::vec4(modelVertexData[i], 1.0f));
				textureCoordinateData.push_back(glm::vec2(modelVertexData[i].x * scalers.x * 0.0025f, modelVertexData[i].y * scalers.y * 0.0025f));
			}

			// fixtures transform
			glm::mat4 fixtureTransform;
			fixtureTransform = glm::translate(fixtureTransform, -0.5f * Utilities::b2Vec2ToGlmVec3(scalers));
			fixtureTransform = glm::scale(fixtureTransform, Utilities::b2Vec2ToGlmVec3(scalers));
			glm::quat fixtureRotationQuaternion;
			fixtureRotationQuaternion = glm::rotate(fixtureRotationQuaternion, tileTransforms[tileType].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			fixtureTransform = fixtureTransform * glm::toMat4(fixtureRotationQuaternion);
			fixtureTransform = glm::translate(fixtureTransform, tileTransforms[tileType].translation);
			for (unsigned int i = 0; i < lowerFixture.size(); ++i) {
				glm::vec3 v = Utilities::b2Vec2ToGlmVec3(lowerFixture[i]);
				lowerFixture[i] = Utilities::glmVec3ToB2Vec2(glm::vec3(fixtureTransform * glm::vec4(v, 1.0f)));
			}
			for (unsigned int i = 0; i < centerFixture.size(); ++i) {
				glm::vec3 v = Utilities::b2Vec2ToGlmVec3(centerFixture[i]);
				centerFixture[i] = Utilities::glmVec3ToB2Vec2(glm::vec3(fixtureTransform * glm::vec4(v, 1.0f)));
			}
			for (unsigned int i = 0; i < upperFixture.size(); ++i) {
				glm::vec3 v = Utilities::b2Vec2ToGlmVec3(upperFixture[i]);
				upperFixture[i] = Utilities::glmVec3ToB2Vec2(glm::vec3(fixtureTransform * glm::vec4(v, 1.0f)));
			}

			break;
		}
		case LEFT_BOTTOM_90:
		case LEFT_TOP_90:
		case TOP_LEFT_90:
		case TOP_RIGHT_90:
		case RIGHT_TOP_90:
		case RIGHT_BOTTOM_90:
		case BOTTOM_LEFT_90:
		case BOTTOM_RIGHT_90: {

			unsigned int curveResolution = 10;

			// bottom
			lowerFixture.push_back(b2Vec2(0.0f, 0.0f));
			for (unsigned int i = 0; i < curveResolution; ++i) {

				float x = 0.4f * glm::cos((glm::half_pi<float>() / curveResolution) * i);
				float y = 0.4f * glm::sin((glm::half_pi<float>() / curveResolution) * i);
				float nextX = 0.4f * glm::cos((glm::half_pi<float>() / curveResolution) * (i + 1));
				float nextY = 0.4f * glm::sin((glm::half_pi<float>() / curveResolution) * (i + 1));

				modelVertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				modelVertexData.push_back(glm::vec3(x, y, 0.0f));
				modelVertexData.push_back(glm::vec3(nextX, nextY, 0.0f));

				lowerFixture.push_back(b2Vec2(x, y));
			}
			lowerFixture.push_back(b2Vec2(0.0f, 0.4f));

			// top
			modelVertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.0f, 0.6f, 0.0f));
			upperFixture.push_back(b2Vec2(1.0f, 1.0f));
			upperFixture.push_back(b2Vec2(0.0f, 1.0f));
			for (unsigned int i = curveResolution; i > 0; --i) {

				float x = 0.6f * glm::cos((glm::half_pi<float>() / curveResolution) * i);
				float y = 0.6f * glm::sin((glm::half_pi<float>() / curveResolution) * i);
				float nextX = 0.6f * glm::cos((glm::half_pi<float>() / curveResolution) * (i - 1));
				float nextY = 0.6f * glm::sin((glm::half_pi<float>() / curveResolution) * (i - 1));

				modelVertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
				modelVertexData.push_back(glm::vec3(x, y, 0.0f));
				modelVertexData.push_back(glm::vec3(nextX, nextY, 0.0f));

				upperFixture.push_back(b2Vec2(x, y));

			}
			modelVertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(0.6f, 0.0f, 0.0f));
			modelVertexData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			upperFixture.push_back(b2Vec2(0.6f, 0.0f));
			upperFixture.push_back(b2Vec2(1.0f, 0.0f));

			// graphics transform
			glm::mat4 transform;
			transform = glm::scale(transform, Utilities::b2Vec2ToGlmVec3(scalers));
			glm::quat rotationQuaternion;
			rotationQuaternion = glm::rotate(rotationQuaternion, tileTransforms[tileType].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			transform = transform * glm::toMat4(rotationQuaternion);
			transform = glm::translate(transform, tileTransforms[tileType].translation);
			for (unsigned int i = 0; i < modelVertexData.size(); ++i) {
				modelVertexData[i] = glm::vec3(transform * glm::vec4(modelVertexData[i], 1.0f));
				textureCoordinateData.push_back(glm::vec2(modelVertexData[i].x * scalers.x * 0.0025f, modelVertexData[i].y * scalers.y * 0.0025f));
			}

			// fixtures transform
			glm::mat4 fixtureTransform;
			fixtureTransform = glm::translate(fixtureTransform, -0.5f * Utilities::b2Vec2ToGlmVec3(scalers));
			fixtureTransform = glm::scale(fixtureTransform, Utilities::b2Vec2ToGlmVec3(scalers));
			glm::quat fixtureRotationQuaternion;
			fixtureRotationQuaternion = glm::rotate(fixtureRotationQuaternion, tileTransforms[tileType].rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			fixtureTransform = fixtureTransform * glm::toMat4(fixtureRotationQuaternion);
			fixtureTransform = glm::translate(fixtureTransform, tileTransforms[tileType].translation);
			for (unsigned int i = 0; i < lowerFixture.size(); ++i) {
				glm::vec3 v = Utilities::b2Vec2ToGlmVec3(lowerFixture[i]);
				lowerFixture[i] = Utilities::glmVec3ToB2Vec2(glm::vec3(fixtureTransform * glm::vec4(v, 1.0f)));
			}
			for (unsigned int i = 0; i < upperFixture.size(); ++i) {
				glm::vec3 v = Utilities::b2Vec2ToGlmVec3(upperFixture[i]);
				upperFixture[i] = Utilities::glmVec3ToB2Vec2(glm::vec3(fixtureTransform * glm::vec4(v, 1.0f)));
			}
		}
	}

	std::string fileName = "resources/microscheme.png";
	generateTexture(fileName);
	transformData.push_back(glm::mat4());

	// push color data
	for (unsigned int i = 0; i < modelVertexData.size(); i++) {
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	initModelVertexBuffer();
	initColorBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | COLOR | TEXTURE_COORDINATE);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoTubeSection::initPhysics() {


	b2BodyDef rigidBodyDef;
	rigidBodyDef.position.Set(position.x + (scalers.x / 2.0f), position.y + (scalers.y / 2.0f));
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);

	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.friction = 0.0f;
	rigidBodyFixtureDef.userData = this;
	rigidBodyFixtureDef.filter.categoryBits = PhysicsManager::CollisionCategory::BOUNDARY;
	rigidBodyFixtureDef.filter.maskBits = gameState->physicsManager->getCollisionMask(PhysicsManager::CollisionCategory::BOUNDARY);
	rigidBodyFixtureDef.restitution = 0.0f;

	if (tileType == LEFT_BOTTOM_90
		|| tileType == LEFT_TOP_90
		|| tileType == TOP_LEFT_90
		|| tileType == TOP_RIGHT_90
		|| tileType == RIGHT_TOP_90
		|| tileType == RIGHT_BOTTOM_90
		|| tileType == BOTTOM_LEFT_90
		|| tileType == BOTTOM_RIGHT_90
		) {

		b2ChainShape rigidBodyShape;
		rigidBodyShape.CreateLoop(lowerFixture.data(), lowerFixture.size());
		rigidBodyFixtureDef.shape = &rigidBodyShape;
		rigidBody->CreateFixture(&rigidBodyFixtureDef);

		rigidBodyShape.Clear();
		rigidBodyShape.CreateLoop(upperFixture.data(), upperFixture.size());
		rigidBodyFixtureDef.shape = &rigidBodyShape;
		rigidBody->CreateFixture(&rigidBodyFixtureDef);
	}
	else {

		b2ChainShape rigidBodyShape;
		rigidBodyShape.CreateLoop(lowerFixture.data(), lowerFixture.size());
		rigidBodyFixtureDef.shape = &rigidBodyShape;
		rigidBodyFixtureDef.isSensor = false;
		rigidBody->CreateFixture(&rigidBodyFixtureDef);

		rigidBodyShape.Clear();
		rigidBodyShape.CreateLoop(centerFixture.data(), centerFixture.size());
		rigidBodyFixtureDef.shape = &rigidBodyShape;
		rigidBodyFixtureDef.isSensor = true;
		b2Fixture* accelFixture = rigidBody->CreateFixture(&rigidBodyFixtureDef);

		rigidBodyShape.Clear();
		rigidBodyShape.CreateLoop(upperFixture.data(), upperFixture.size());
		rigidBodyFixtureDef.shape = &rigidBodyShape;
		rigidBodyFixtureDef.isSensor = false;
		rigidBody->CreateFixture(&rigidBodyFixtureDef);

		//fixtureAccelerations[accelFixture] = b2Vec2(1000.0f, 0.0f);
		fixtureAccelerations[accelFixture] = b2Vec2(0.0f, 0.0f);
	}

}

void PoTubeSection::render() {

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

b2Vec2 PoTubeSection::getFixtureAcceleration(b2Fixture* fixture) {
	return fixtureAccelerations[fixture];
}

PoTubeSection::~PoTubeSection() {
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);
}
