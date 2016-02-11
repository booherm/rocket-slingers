#include "PoRope.hpp"
/*
PoRope::PoRope(GameState* gameState) : PhysicalObject(gameState) {
	initGeometry();
	initShaders();

	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
}

void PoRope::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoRope callback.  InputEvent = " << std::endl;

	// calculate world position from click coordinates
	float worldX = ((float)inputEvent.xCoordinate * gameState->aspectRatio) / gameState->resolutionWidth;
	float worldY = 1.0f - ((float)inputEvent.yCoordinate / gameState->resolutionHeight);
	std::cout << "world space coords (" << worldX << ", " << worldY << ")" << std::endl;

	if (ropeCount < maxRopeCount) {

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
		std::vector<RopeMass> ropeMasses(ropeMassCount);
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

		// add to container of rope masses accross all rope instances for rendering
		allRopeMassesContainer.push_back(ropeMasses);
		ropeCount++;
	}

	inputEvent.print();
}

void PoRope::initGeometry() {

	// init buffers and vertex array object
	colorSize = maxRopeCount * (ropeMassCount - 1);
	colorData.resize(colorSize);
	transformSize = maxRopeCount * (ropeMassCount - 1);
	transformData.resize(transformSize);
	glGenVertexArrays(1, &masterVao);
	glGenBuffers(1, &modelVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);

	// buffer axes model data
	GLfloat ropeModelVertices[] = {1.0f, 0.0f};
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ropeModelVertices), ropeModelVertices, GL_STATIC_DRAW);

	// initialize rope color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colorSize, NULL, GL_STREAM_DRAW);

	// initialize guy transform buffer
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformSize, NULL, GL_STREAM_DRAW);

	// start vertex array object setup
	glBindVertexArray(masterVao);

	// define position attribute (model)
	glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// define color attribute (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	// define transform attribute (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	for (unsigned int i = 2; i <= 5; i++) {
		glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)((i - 2) * sizeof(glm::vec4)));
		glVertexAttribDivisor(i, 1);
		glEnableVertexAttribArray(i);
	}

	// end vertex array object setup
	glBindVertexArray(0);
}

void PoRope::initShaders() {
	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec2 position;\n"
		"layout (location = 1) in vec4 inColor;\n"
		"layout (location = 2) in mat4 transformMatrix;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position, 0.0f, 1.0f);\n"
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

void PoRope::updatePhysicalState() {

	if (ropeCount == 0)
		return;

	transformSize = 0;
	colorSize = 0;
	//float ropeColorGrad = 0.75f / (ropeMassCount - 1);  // gradient rope color for visual debugging

	// Iterate over all ropes
	for (unsigned int ropeIndex = 0; ropeIndex < ropeCount; ropeIndex++) {

		// reset forces
		for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {
			RopeMass* thisMass = &allRopeMassesContainer[ropeIndex].at(massIndex);
			thisMass->force = glm::vec3(0.0f, 0.0f, 0.0f);
		}


		// Iterate over rope masses of this rope, starting from the top anchor point.  No work to do
		// on the last mass as it does not have any spring connections below it.
		for (unsigned int massIndex = 0; massIndex < ropeMassCount - 1; massIndex++) {

			// get this rope mass and it's immediate neighbor down the rope
			RopeMass* thisMass = &allRopeMassesContainer[ropeIndex].at(massIndex);
			RopeMass* nextMass = &allRopeMassesContainer[ropeIndex].at(massIndex + 1);

			// apply gravitational force to segment
			//thisSegment->force += glm::vec3(0.0f, -0.01f, 0.0f);

			// calculate spring force between this mass its neighbor mass down the rope
			glm::vec3 springVector = thisMass->position - nextMass->position;
			thisMass->stretchedLength = glm::length(springVector);

//				std::cout << "segment " << segmentIndex << " springVector = " << Utilities::vectorToString(springVector) << ", length = " << thisSegment->stretchedLength << std::endl;

			glm::vec3 springForce;
			if (thisMass->stretchedLength != 0) {
//				std::cout << "segment " << segmentIndex << " springVector / stretched length = " << Utilities::vectorToString(springVector / thisSegment->stretchedLength) << std::endl;
//					std::cout << "segment " << segmentIndex << " stretchedLength - unstretchedLength = " << (stretchedLength - unstretchedLength) << std::endl;
				//springForce = (springVector / thisSegment->stretchedLength) * (thisSegment->stretchedLength - thisSegment->unstretchedLength) * -springStiffnessConstant;
				springForce = -(springVector / thisMass->stretchedLength) * (thisMass->stretchedLength - thisMass->unstretchedLength) * springStiffnessConstant;
			}

			// apply internal spring friction
			springForce += -(thisMass->velocity - nextMass->velocity) * internalSpringFrictionConstant;

			// apply net spring force to this rope segment
			thisMass->force += springForce;

			// apply opposite net force to adjacent rope segment.
			nextMass->force += -springForce;



			// update rope segment velocity and position, if not the first segment
			//if (segmentIndex > 0) {
				//thisSegment->velocity += ((thisSegment->force / thisSegment->mass) * gameState->fLastFrameTotalTime);
				//thisSegment->position += (thisSegment->velocity * gameState->fLastFrameTotalTime);
			//}

		}

		for (unsigned int massIndex = 0; massIndex < ropeMassCount; massIndex++) {

			////// physics have been applied, now rendering attributes
			RopeMass* thisMass = &allRopeMassesContainer[ropeIndex].at(massIndex);
			RopeMass* nextMass = nullptr;
			if (massIndex != ropeMassCount - 1)   // last segment has no downward neighbor
				nextMass = &allRopeMassesContainer[ropeIndex].at(massIndex + 1);

			// apply gravitational force to segment
			thisMass->force += glm::vec3(0.0f, -1.0f, 0.0f) * thisMass->mass;

			// apply air friction force
			thisMass->force += (-thisMass->velocity * airFrictionConstant);

			if (massIndex > 0) { // do not update velocity or position on the anchor mass
				thisMass->velocity += ((thisMass->force / thisMass->mass) * gameState->fLastFrameTotalTime);
				thisMass->position += (thisMass->velocity * gameState->fLastFrameTotalTime);
			}

//			if(segmentIndex == 1)
//				std::cout << "Segment " << segmentIndex << " new velocity = " << Utilities::vectorToString(thisSegment->velocity) << std::endl;

			if (nextMass != nullptr) {
				// color
				//glm::vec4 ropeColorVector(0.25f + (massIndex * ropeColorGrad), 0.25f + (massIndex * ropeColorGrad), 0.25f + (massIndex * ropeColorGrad), 1.0f);
				glm::vec4 ropeColorVector(1.0f, 1.0f, 1.0f, 1.0f);
				colorData[colorSize++] = ropeColorVector;

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
				transformData[transformSize++] = transform;
			}
		}	
	}

	// buffer color data
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * colorSize, colorData.data());

	// buffer transform data
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * transformSize, transformData.data());
}

void PoRope::render() {
	shaderProg.use();
	glBindVertexArray(masterVao);
	glDrawArraysInstanced(GL_LINES, 0, 2, (ropeMassCount - 1) * ropeCount);  // 2 vertices per rope segment, draw total count of rope segments
}

PoRope::~PoRope() {
	glDeleteBuffers(1, &modelVbo);
	glDeleteBuffers(1, &transformVbo);
	glDeleteVertexArrays(1, &masterVao);
}
*/