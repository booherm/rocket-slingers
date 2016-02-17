#include "PoPhysicsRenderer.hpp"

PoPhysicsRenderer::PoPhysicsRenderer(GameState* gameState) : PhysicalObject("PO_PHYSICS_RENDERER", gameState) {
	initGeometry();
	initShaders();
	glRenderingMode = GL_LINES;
	shouldRender = true;
	gameState->physicalObjectRenderer->addPhysicalObject(this);
	gameState->physicsManager->setDebugRenderer(this);
}

void PoPhysicsRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {

	
	glm::vec3 fromVec(from.x(), from.y(), from.z());
	glm::vec3 toVec(to.x(), to.y(), to.z());
	//glm::vec4 colorVec(color.x(), color.y(), color.z(), 1.0f);
	glm::vec4 colorVec(1.0f, 1.0f, 1.0f, 1.0f);

	colorData.push_back(colorVec);
	colorData.push_back(colorVec);

	// transform
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, fromVec);

	// calculate angle between this segment and the next segment
	float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), fromVec - toVec) - glm::pi<float>();

	// calculate distance between end points of this segment and the next segment
	float distance = glm::distance(fromVec, toVec);

	// scale by distance
	modelTransform = glm::scale(modelTransform, glm::vec3(distance, distance, 1.0f));

	// rotate by theta
	glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotationQuaternionMatrix = glm::toMat4(rotationQuaternion);
	modelTransform = modelTransform * rotationQuaternionMatrix;

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	transformData.push_back(transform);

}

void PoPhysicsRenderer::flushLines() {
}

void PoPhysicsRenderer::afterRender() {
	transformData.clear();
	colorData.clear();
}

void PoPhysicsRenderer::doRenderUpdate() {

}

void PoPhysicsRenderer::sdlInputEventCallback(const Event& eventObj) {

}

void PoPhysicsRenderer::initGeometry() {
	// model data
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

PoPhysicsRenderer::~PoPhysicsRenderer() {};

void PoPhysicsRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
};

void PoPhysicsRenderer::reportErrorWarning(const char* warningString) {
};

void PoPhysicsRenderer::draw3dText(const btVector3& location, const char* textString) {
};

void PoPhysicsRenderer::setDebugMode(int debugMode) {
};

int	 PoPhysicsRenderer::getDebugMode() const {
	return 1;
};

//void PoPhysicsRenderer::setDefaultColors(const DefaultColors&) {
//
//}
