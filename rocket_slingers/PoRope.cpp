#include "PoRope.hpp"

PoRope::PoRope(GameState* gameState) : PhysicalObject("PO_ROPE", gameState) {
	initGeometry();
	initShaders();

	maxAllowedChangeInTime = 0.002f;
	this->glRenderingMode = GL_LINES;
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_KEYDOWN, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoRope::inputEventCallback(InputEvent inputEvent) {

	if (inputEvent.eventKey == InputEvent::IEK_MOUSE_BUTTON_1) {

		// determine rope anchor and termination coordinates
		glm::vec3 ropeAnchorPoint(inputEvent.xWorldCoordinate, inputEvent.yWorldCoordinate, 0.0f);
		glm::vec3 ropeTerminationPoint(10.0f, 15.0f, 0.0f);  // this is a temporary hard-coded point, would get from player position

		// calculate attributes that will apply to all rope masses
		float overallRopeLength = glm::distance(ropeAnchorPoint, ropeTerminationPoint);
		float ropeSegmentLength = overallRopeLength / (ropeMassCount - 1);
		float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), ropeAnchorPoint - ropeTerminationPoint);
		float positionOffsetX = glm::cos(theta) * ropeSegmentLength;
		float positionOffsetY = glm::sin(theta) * ropeSegmentLength;

		// Create rope masses.  Each rope mass is a point on the rope joined by a spring segment to it's immediate neighbor
		// down the rope.  The first and last masses on the rope only have internal spring segments.
		componentMasses.resize(ropeMassCount);
		ropeSegmentLengths.resize(ropeMassCount);
		for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {

			PhysicalMass* ropeMass = &componentMasses[massIndex];
			ropeMass->mass = ropeMassMass;
			RopeSegmentLength* rsl = &ropeSegmentLengths[massIndex];
			rsl->unstretchedLength = ropeSegmentLength;
			rsl->stretchedLength = ropeSegmentLength;

			// determine mass origin position
			ropeMass->worldPosition = glm::vec3(ropeAnchorPoint.x - (positionOffsetX * massIndex),
				ropeAnchorPoint.y - (positionOffsetY * massIndex), 0.0f);

		}

		shouldRender = true;
		shouldDoPhysicalUpdate = true;
	}
	else
	{
		switch (inputEvent.eventKey) {
			case InputEvent::IEK_KEY_Q:
				ropeMassCount++;
				colorData.clear();
				for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {
					colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				}
				break;
			case InputEvent::IEK_KEY_W:
				ropeMassMass += 0.25f;
				break;
			case InputEvent::IEK_KEY_E:
				springStiffnessConstant += 100.0f;
				break;
			case InputEvent::IEK_KEY_R:
				internalSpringFrictionConstant += 1.0f;
				break;
			case InputEvent::IEK_KEY_T:
				airFrictionConstant += 1.0f;
				break;
			case InputEvent::IEK_KEY_Y:
				gravitationalConstant += 0.5f;
				break;
			case InputEvent::IEK_KEY_A:
				ropeMassCount--;
				colorData.clear();
				for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {
					colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				}
				break;
			case InputEvent::IEK_KEY_S:
				ropeMassMass -= 0.25f;
				break;
			case InputEvent::IEK_KEY_D:
				springStiffnessConstant -= 100.0f;
				break;
			case InputEvent::IEK_KEY_F:
				internalSpringFrictionConstant -= 1.0f;
				break;
			case InputEvent::IEK_KEY_G:
				airFrictionConstant -= 1.0f;
				break;
			case InputEvent::IEK_KEY_H:
				gravitationalConstant -= 0.5f;
				break;
		}

		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << "Rope variables state:" << std::endl;
		std::cout << "                 ropeMassCount = " << ropeMassCount << std::endl;
		std::cout << "                  ropeMassMass = " << ropeMassMass << std::endl;
		std::cout << "       springStiffnessConstant = " << springStiffnessConstant << std::endl;
		std::cout << "internalSpringFrictionConstant = " << internalSpringFrictionConstant << std::endl;
		std::cout << "           airFrictionConstant = " << airFrictionConstant << std::endl;
		std::cout << "         gravitationalConstant = " << gravitationalConstant << std::endl;
	}
}

void PoRope::initGeometry() {
	// model data
	this->modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	this->modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void PoRope::doPhysicalUpdate() {

	resetForces();

	// iterate over rope masses of this rope, applying fores starting from the top anchor point
	for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {

		// get this rope mass and it's immediate neighbor down the rope
		PhysicalMass* thisMass = &componentMasses[massIndex];
		RopeSegmentLength* thisMassSegmentLengths = &ropeSegmentLengths[massIndex];
		PhysicalMass* nextMass = nullptr;
		if (massIndex != ropeMassCount - 1)   // last segment has no downward neighbor
			nextMass = &componentMasses[massIndex + 1];

		if (nextMass != nullptr) {
			// calculate and apply spring force
			glm::vec3 springVector = thisMass->worldPosition - nextMass->worldPosition;
			thisMassSegmentLengths->stretchedLength = glm::length(springVector);
			glm::vec3 springForce;
			if (thisMassSegmentLengths->stretchedLength != 0) {
				springForce = -(springVector / thisMassSegmentLengths->stretchedLength)
					* (thisMassSegmentLengths->stretchedLength - thisMassSegmentLengths->unstretchedLength)
					* springStiffnessConstant;
			}

			// apply internal spring friction
			springForce += -(thisMass->velocity - nextMass->velocity) * internalSpringFrictionConstant;

			// apply spring force to this rope segment
			thisMass->force += springForce;

			// apply opposite spring force to adjacent mass
			nextMass->force += -springForce;
		}

		// apply gravitational force
		thisMass->force += thisMass->mass * glm::vec3(0.0f, -gravitationalConstant, 0.0f);

		// apply air friction force
		thisMass->force += (-thisMass->velocity * airFrictionConstant);

		if (massIndex > 0) { // do not update velocity or position on the anchor mass
			thisMass->velocity += ((thisMass->force / thisMass->mass) * changeInTime);
			thisMass->worldPosition += (thisMass->velocity * changeInTime);
		}
	}

}

void PoRope::doRenderUpdate() {

	transformData.clear();
	for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {
		PhysicalMass* thisMass = &componentMasses[massIndex];
		PhysicalMass* nextMass = &componentMasses[massIndex + 1];

		// transform
		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, thisMass->worldPosition);

		// calculate angle between this segment and the next segment
		float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), thisMass->worldPosition - nextMass->worldPosition) - glm::pi<float>();

		// calculate distance between end points of this segment and the next segment
		float distance = glm::distance(thisMass->worldPosition, nextMass->worldPosition);

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