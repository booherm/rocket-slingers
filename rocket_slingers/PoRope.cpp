#include "PoRope.hpp"

PoRope::PoRope(GameState* gameState) : PhysicalObject("PO_ROPE", gameState) {
	initGeometry();
	initShaders();

	this->glRenderingMode = GL_LINES;
	shouldRender = false;
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoRope::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoRope callback.  InputEvent = " << std::endl;

	// calculate world position from click coordinates
	float worldX = ((float)inputEvent.xCoordinate * gameState->aspectRatio) / gameState->resolutionWidth;
	float worldY = 1.0f - ((float)inputEvent.yCoordinate / gameState->resolutionHeight);
	std::cout << "world space coords (" << worldX << ", " << worldY << ")" << std::endl;

	// determine rope anchor and termination coordinates
	glm::vec3 ropeAnchorPoint(worldX, worldY, 0.0f);
	glm::vec3 ropeTerminationPoint(1.25f, 0.9f, 0.0f);  // this is a temporary hard-coded point, would get from player position

	// calculate attributes that will apply to all rope masses
	float overallRopeLength = glm::distance(ropeAnchorPoint, ropeTerminationPoint);
	float ropeSegmentLength = overallRopeLength / (ropeMassCount - 1);
	float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), ropeAnchorPoint - ropeTerminationPoint);
	float positionOffsetX = glm::cos(theta) * ropeSegmentLength;
	float positionOffsetY = glm::sin(theta) * ropeSegmentLength;

	// Create rope masses.  Each rope mass is a point on the rope joined by a spring segment to it's immediate neighbor
	// down the rope.  The first and last masses on the rope only have internal spring segments.
	ropeMasses.resize(ropeMassCount);
	for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {

		RopeMass ropeMass;
		ropeMass.mass = 10.05f;
		ropeMass.unstretchedLength = ropeSegmentLength;

		// determine mass origin position
		ropeMass.position = glm::vec3(ropeAnchorPoint.x - (positionOffsetX * massIndex),
			ropeAnchorPoint.y - (positionOffsetY * massIndex), 0.0f);
			
		// store rope mass
		ropeMasses[massIndex] = ropeMass;
	}

	shouldRender = true;
}

void PoRope::initGeometry() {

	/*
	// init buffers and vertex array object
	colorSize = maxRopeCount * (ropeMassCount - 1);
	colorData.resize(colorSize);
	transformSize = maxRopeCount * (ropeMassCount - 1);
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);
	*/

	// model data
	this->modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	this->modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

void PoRope::updatePhysicalState() {

	if (!shouldRender)
		return;

	modelOriginOffsetData.clear();
	colorData.clear();
	transformData.clear();


	//float ropeColorGrad = 0.75f / (ropeMassCount - 1);  // gradient rope color for visual debugging

	// reset forces
	for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {
		RopeMass* thisMass = &ropeMasses[massIndex];
		thisMass->force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	
	// Iterate over rope masses of this rope, starting from the top anchor point.  No work to do
	// on the last mass as it does not have any spring connections below it.
	for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {

		// get this rope mass and it's immediate neighbor down the rope
		RopeMass* thisMass = &ropeMasses[massIndex];
		RopeMass* nextMass = &ropeMasses[massIndex + 1];

		// apply gravitational force to segment
		//thisSegment->force += glm::vec3(0.0f, -0.01f, 0.0f);

		// calculate spring force between this mass its neighbor mass down the rope
		glm::vec3 springVector = thisMass->position - nextMass->position;
		thisMass->stretchedLength = glm::length(springVector);
		glm::vec3 springForce;
		if (thisMass->stretchedLength != 0) {
			springForce = -(springVector / thisMass->stretchedLength) * (thisMass->stretchedLength - thisMass->unstretchedLength) * springStiffnessConstant;
		}

		// apply internal spring friction
		springForce += -(thisMass->velocity - nextMass->velocity) * internalSpringFrictionConstant;

		// apply net spring force to this rope segment
		thisMass->force += springForce;

		// apply opposite net force to adjacent rope segment.
		nextMass->force += -springForce;
	}


	// update rope segment velocity and position, if not the first segment
	//if (segmentIndex > 0) {
		//thisSegment->velocity += ((thisSegment->force / thisSegment->mass) * gameState->fLastFrameTotalTime);
		//thisSegment->position += (thisSegment->velocity * gameState->fLastFrameTotalTime);
	//}

	for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {

		////// physics have been applied, now rendering attributes
		RopeMass* thisMass = &ropeMasses[massIndex];
		RopeMass* nextMass = nullptr;
		if (massIndex != ropeMassCount - 1)   // last segment has no downward neighbor
			nextMass = &ropeMasses[massIndex + 1];

		// apply gravitational force to segment
		thisMass->force += glm::vec3(0.0f, -1.0f, 0.0f) * thisMass->mass;

		// apply air friction force
		thisMass->force += (-thisMass->velocity * airFrictionConstant);

		if (massIndex > 0) { // do not update velocity or position on the anchor mass
			thisMass->velocity += ((thisMass->force / thisMass->mass) * gameState->fLastFrameTotalTime);
			thisMass->position += (thisMass->velocity * gameState->fLastFrameTotalTime);
		}

		if (nextMass != nullptr) {
			// color
			//glm::vec4 ropeColorVector(0.25f + (massIndex * ropeColorGrad), 0.25f + (massIndex * ropeColorGrad), 0.25f + (massIndex * ropeColorGrad), 1.0f);
			glm::vec4 ropeColorVector(1.0f, 1.0f, 1.0f, 1.0f);
			colorData.push_back(ropeColorVector);
			modelOriginOffsetData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

			// model
			glm::mat4 modelTransform;
			modelTransform = glm::translate(modelTransform, thisMass->position);

			// calculate angle between this segment and the next segment
			float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), thisMass->position - nextMass->position) - glm::pi<float>();

			// calculate distance between end points of this segment and the next segment
			float distance = glm::distance(thisMass->position, nextMass->position);

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
	}	

}

/*
void PoRope::render() {
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_LINES, 0, 2, (ropeMassCount - 1) * ropeCount);  // 2 vertices per rope segment, draw total count of rope segments
}
*/
