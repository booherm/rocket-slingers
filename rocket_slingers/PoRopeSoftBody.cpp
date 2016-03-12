#include "PoRopeSoftBody.hpp"
#include <BulletSoftBody/btSoftBodyHelpers.h>

PoRopeSoftBody::PoRopeSoftBody(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	ropeStartPosition = glm::vec3(15.0f, 15.0f, 0.0f);
	ropeEndPosition = glm::vec3(25.0f, 15.0f, 0.0f);
	initalRopeSegments = 10;
	maxRopeSegments = 10;
	maxRopeSegmentLength = glm::distance(ropeStartPosition, ropeEndPosition) / initalRopeSegments;


	initShaders();
	initGeometry();
	initPhysics();

	shouldDoPhysicalUpdate = true;
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
}


void PoRopeSoftBody::gameEventCallback(const Event& eventObj) {

	player = (PoGuy*) eventObj.eventPoster;
	bool attachedToStructure = true;

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {

	
		// play rope deployment sound effect
		gameState->audioManager->playSoundEffect(attachedToStructure ? AudioManager::SoundEffectId::ROPE_HIT : AudioManager::SoundEffectId::ROPE_MISS, 0);
		shouldDoPhysicalUpdate = true;
	}
	else {
	
		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);
		shouldDoPhysicalUpdate = false;
	}

	player->setRopeAttachedToStructure(attachedToStructure);
}


void PoRopeSoftBody::initShaders() {

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

void PoRopeSoftBody::initGeometry() {

	glRenderingMode = GL_LINES;
	initModelVertexBuffer(10000);
	initColorBuffer(10000);
	buildVao(MODEL_VERTEX | COLOR);

	shouldRender = true;
	gameState->masterRenderer->addRenderableObject(this);
}

void PoRopeSoftBody::initPhysics() {

	/*
	float s = 4.0f;
	float h = 20.0f;

	ropeSoftBody = btSoftBodyHelpers::CreatePatch(
		gameState->physicsManager->dynamicsWorld->getWorldInfo(),
		btVector3(-s, h, -s),
		btVector3(s, h, -s),
		btVector3(-s, h, s),
		btVector3(s, h, s),
		10,
		10,
		4 + 8,
		true
	);
	*/

	glm::mat4 transform;
	transform = glm::translate(transform, ropeEndPosition);
	bob = new PhysicalMass();
	bob->init("PO_SOFT_BODY_ROPE_BOB", gameState, 64.0f, transform, PhysicsManager::CollisionGroup::NO_COLLISION);
	bob->addCollisionShapeSphere(glm::mat4(), 1.0f);
	bob->addToDynamicsWorld();

	btVector3 ropeStartPositionBt;
	btVector3 ropeEndPositionBt;
	PhysicsManager::glmVec3ToBtVec3(ropeStartPosition, ropeStartPositionBt);
	PhysicsManager::glmVec3ToBtVec3(ropeEndPosition, ropeEndPositionBt);
	unsigned int resolution = initalRopeSegments - 1;
	ropeSoftBody = btSoftBodyHelpers::CreateRope(gameState->physicsManager->dynamicsWorld->getWorldInfo(), ropeStartPositionBt, ropeEndPositionBt, resolution, 1);
	ropeSoftBody->appendAnchor(resolution + 1, bob->rigidBody, true, 1.0f);
	//ropeSoftBody->setRestitution(0.01f); // no effect
	//ropeSoftBody->setRestitution(100.0f); // no effect
	//ropeSoftBody->setRestLengthScale(100.0f);// no effect
	//ropeSoftBody->setRestLengthScale(0.01f);// no effect
	//ropeSoftBody->setFriction(10000.0f);// no effect
	//ropeSoftBody->setFriction(0.01f);// no effect
	//ropeSoftBody->setRollingFriction(0.01f);
	//ropeSoftBody->setRollingFriction(10000.0
	
	for (int i = 0; i < ropeSoftBody->m_materials.size(); i++) {
//		std::cout << "adjusting stiffness " << i << std::endl;
		ropeSoftBody->m_materials.at(i)->m_kLST = 1.0f;
		//ropeSoftBody->m_materials.at(i)->m_kAST = 1.0f;
		//ropeSoftBody->m_materials.at(i)->m_kVST = 0.0f;
	}
	

	for (int i = 1; i < ropeSoftBody->m_nodes.size(); i++) {
		//ropeSoftBody->setMass(i, 5.0f * i);
		//ropeSoftBody->setMass(i, 1.0f / i);
		//ropeSoftBody->setMass(i, 1000.0f / (i * i));
		//ropeSoftBody->setMass(i, 1000.0f);
		ropeSoftBody->setMass(i, 1.0f);
	}

	for (int i = 0; i < ropeSoftBody->m_links.size(); i++) {
		ropeSoftBody->m_links[i].m_rl = maxRopeSegmentLength;
	}
	
	/*
	ropeSoftBody->updateNormals();
	ropeSoftBody->updateBounds();
	ropeSoftBody->updateArea();
	ropeSoftBody->updateClusters();
	ropeSoftBody->updatePose();
	ropeSoftBody->updateLinkConstants();
	ropeSoftBody->updateConstants();
	ropeSoftBody->setTotalMass(10.0f);
	*/
	//ropeSoftBody->setRestLengthScale(0.5f);
	gameState->physicsManager->dynamicsWorld->addSoftBody(ropeSoftBody);
	
/*
for (int i = 0; i < ropeSoftBody->m_materials.size(); i++) {
		std::cout << "adjusting stiffness " << i << std::endl;
				ropeSoftBody->m_materials.at(i)->m_kLST = 0.0f;
		//ropeSoftBody->m_materials.at(i)->m_kAST = 0.0f;
		//		ropeSoftBody->m_materials.at(i)->m_kVST = 0.0f;
	}

	//ropeSoftBody->m_cfg.citerations = 20;
	//ropeSoftBody->m_cfg.diterations = 20;
	//ropeSoftBody->m_cfg.kDP = 0.1f;
	//ropeSoftBody->m_cfg.kDF = 1.0f;
	//ropeSoftBody->m_cfg.kDG = 1000000.0f;
	//ropeSoftBody->m_cfg.piterations = 10000;
	ropeSoftBody->updateNormals();
	ropeSoftBody->updateBounds();
	ropeSoftBody->updateArea();
	ropeSoftBody->updateClusters();
	ropeSoftBody->updatePose();
	ropeSoftBody->updateLinkConstants();
	ropeSoftBody->updateConstants();
	*/

	std::cout << "m_cfg.citerations " << ropeSoftBody->m_cfg.citerations << std::endl;
	std::cout << "m_cfg.diterations " << ropeSoftBody->m_cfg.diterations << std::endl;
	std::cout << "m_cfg.kAHR " << ropeSoftBody->m_cfg.kAHR << std::endl;
	std::cout << "m_cfg.kCHR " << ropeSoftBody->m_cfg.kCHR << std::endl;
	std::cout << "m_cfg.kDF " << ropeSoftBody->m_cfg.kDF << std::endl;
	std::cout << "m_cfg.kDG " << ropeSoftBody->m_cfg.kDG << std::endl;
	std::cout << "m_cfg.kDP " << ropeSoftBody->m_cfg.kDP << std::endl;
	std::cout << "m_cfg.kKHR " << ropeSoftBody->m_cfg.kKHR << std::endl;
	std::cout << "m_cfg.kLF " << ropeSoftBody->m_cfg.kLF << std::endl;
	std::cout << "m_cfg.kMT " << ropeSoftBody->m_cfg.kMT << std::endl;
	std::cout << "m_cfg.kPR " << ropeSoftBody->m_cfg.kPR << std::endl;
	std::cout << "m_cfg.kSHR " << ropeSoftBody->m_cfg.kSHR << std::endl;
	std::cout << "m_cfg.kSKHR_CL " << ropeSoftBody->m_cfg.kSKHR_CL << std::endl;
	std::cout << "m_cfg.kSK_SPLT_CL " << ropeSoftBody->m_cfg.kSK_SPLT_CL << std::endl;
	std::cout << "m_cfg.kSRHR_CL " << ropeSoftBody->m_cfg.kSRHR_CL << std::endl;
	std::cout << "m_cfg.kSR_SPLT_CL " << ropeSoftBody->m_cfg.kSR_SPLT_CL << std::endl;
	std::cout << "m_cfg.kSSHR_CL " << ropeSoftBody->m_cfg.kSSHR_CL << std::endl;
	std::cout << "m_cfg.kSS_SPLT_CL " << ropeSoftBody->m_cfg.kSS_SPLT_CL << std::endl;
	std::cout << "m_cfg.kVC " << ropeSoftBody->m_cfg.kVC << std::endl;
	std::cout << "m_cfg.kVCF " << ropeSoftBody->m_cfg.kVCF << std::endl;
	std::cout << "m_cfg.piterations " << ropeSoftBody->m_cfg.piterations << std::endl;
	std::cout << "m_cfg.timescale " << ropeSoftBody->m_cfg.timescale << std::endl;
	std::cout << "m_cfg.viterations " << ropeSoftBody->m_cfg.viterations << std::endl;

	/*
	std::cout << "m_anchors.size() = " << ropeSoftBody->m_anchors.size() << std::endl;
	std::cout << "m_clusterConnectivity.size() = " << ropeSoftBody->m_clusterConnectivity.size() << std::endl;
	std::cout << "m_clusters.size() = " << ropeSoftBody->m_clusters.size() << std::endl;
	std::cout << "m_collisionDisabledObjects.size() = " << ropeSoftBody->m_collisionDisabledObjects.size() << std::endl;
	std::cout << "m_faces.size() = " << ropeSoftBody->m_faces.size() << std::endl;
	std::cout << "m_joints.size() = " << ropeSoftBody->m_joints.size() << std::endl;
	std::cout << "m_links.size() = " << ropeSoftBody->m_links.size() << std::endl;
	std::cout << "m_materials.size() = " << ropeSoftBody->m_materials.size() << std::endl;
	std::cout << "m_nodes.size() = " << ropeSoftBody->m_nodes.size() << std::endl;
	std::cout << "m_notes.size() = " << ropeSoftBody->m_notes.size() << std::endl;
	std::cout << "m_rcontacts.size() = " << ropeSoftBody->m_rcontacts.size() << std::endl;
	std::cout << "m_restLengthScale = " << ropeSoftBody->m_restLengthScale << std::endl;
	std::cout << "m_scontacts.size() = " << ropeSoftBody->m_scontacts.size() << std::endl;
	std::cout << "m_tetras.size() = " << ropeSoftBody->m_tetras.size() << std::endl;
	std::cout << "m_userIndexMapping.size() = " << ropeSoftBody->m_userIndexMapping.size() << std::endl;
	*/
}

void PoRopeSoftBody::doPhysicalUpdate() {


	int upperIndex = ropeSoftBody->m_links.size() - 1;
	//setSegmentLength(0, upperIndex, maxRopeSegmentLength);
	//setSegmentLength(0, upperIndex, 10.0f);
	setSegmentLength(0, upperIndex, 5.0f);

	/*
	glm::vec3 pos;
	bob->getCenterOfMassPosition(pos);

	glm::vec3 basePosition;
	btVector3 basePositionBt = ropeSoftBody->m_nodes[0].m_x;
	PhysicsManager::btVec3ToGlmVec3(basePositionBt, basePosition);

	pos = pos - basePosition;

	// get impulse from player rocket
	glm::vec3 rocketForce;
	player->getRocketImpulse(rocketForce);
	float forceMagnitude = glm::length(rocketForce);
	if (rocketForce.x > 0.0f)
		forceMagnitude = -forceMagnitude;

	// rotate x normal vector to tangent position
	float theta;
	if (pos.x == 0.0f) {
		if (pos.y > 0.0f)
			theta = glm::half_pi<float>();
		else
			theta = glm::three_over_two_pi<float>();
	}
	else {
		theta = glm::atan(pos.y / pos.x);

		if (pos.x > 0.0f) {
			if (pos.y < 0.0f)
				theta += glm::two_pi<float>();
		}
		else {
			if (pos.y == 0.0f)
				theta = glm::pi<float>();
			else
				theta += glm::pi<float>();
		}
	}
	glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 radialVector = rotationQuaternion * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 perpForce(-radialVector.y, radialVector.x, radialVector.z);
	perpForce = perpForce * forceMagnitude;

	//std::cout << "pos = " << Utilities::vectorToString(pos) << ", theta = " << theta << ", radialVector = " << Utilities::vectorToString(radialVector) << ", perpForce = " << Utilities::vectorToString(perpForce) << std::endl;

	btVector3 perpForceBt;
	PhysicsManager::glmVec3ToBtVec3(perpForce, perpForceBt);
	//ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody->applyCentralForce(perpForceBt);
	bob->rigidBody->applyCentralImpulse(perpForceBt * 1.0f);

	//ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody->applyCentralImpulse(perpForceBt);
	*/

}

void PoRopeSoftBody::render() {

	modelVertexData.clear();
	colorData.clear();

	for (int i = 0; i < ropeSoftBody->m_links.size(); i++) {
		for (int j = 0; j < 2; j++) {
			modelVertexData.push_back(glm::vec3(
				ropeSoftBody->m_links[i].m_n[j]->m_x.x(),
				ropeSoftBody->m_links[i].m_n[j]->m_x.y(),
				ropeSoftBody->m_links[i].m_n[j]->m_x.z()
			));
			colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	refreshModelVertexBuffer();
	refreshColorBuffer();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}

PoRopeSoftBody::~PoRopeSoftBody() {
	gameState->physicsManager->dynamicsWorld->removeSoftBody(ropeSoftBody);
	delete ropeSoftBody;

	delete bob;
}




void PoRopeSoftBody::setSegmentLength(const int& idxFirstLink, const int& idxLastLink, const btScalar& length)
{

	//ropeSoftBody->updateNormals();
	//ropeSoftBody->updateBounds();
	//ropeSoftBody->updateConstants();
	//ropeSoftBody->setTotalMass(mass);

	float mass = 10.0f;

	// if length equals current length -> return
	float initialSegmentLength = getSegmentLength(idxFirstLink, idxLastLink);
	if (length - initialSegmentLength == 0.0)
	{
		return;
	}

	// maxRopeSegments: maximum number of rope segments
	// maxRopeSegmentLength: maximum length of one segment
	if (length > maxRopeSegments * maxRopeSegmentLength)
	{ /// too long to accommodate with maxSegmentSize-sized segments
		btScalar segmentSize = length / maxRopeSegments;
		for (int i = idxFirstLink; i <= idxLastLink; i++)
		{
			ropeSoftBody->m_nodes[i + 1].m_x
				= lerp(ropeSoftBody->m_nodes[i].m_x, ropeSoftBody->m_nodes[i + 1].m_x,
					segmentSize / ropeSoftBody->m_links[i].m_rl);
			ropeSoftBody->m_nodes[i + 1].m_q
				= lerp(ropeSoftBody->m_nodes[i].m_q, ropeSoftBody->m_nodes[i + 1].m_q,
					segmentSize / ropeSoftBody->m_links[i].m_rl);
			ropeSoftBody->m_links[i].m_rl = segmentSize;
		}
		ropeSoftBody->updateNormals();
		ropeSoftBody->updateBounds();
		ropeSoftBody->updateConstants();
		ropeSoftBody->setTotalMass(mass);
		return;
	}
	else
	{
		for (int i = idxFirstLink; i <= idxLastLink; i++)
		{
			if (ropeSoftBody->m_links[i].m_rl > maxRopeSegmentLength)
			{
				btVector3 thisNodePosition = ropeSoftBody->m_nodes[i].m_x;
				btVector3 nextNodePosition = ropeSoftBody->m_nodes[i + 1].m_x;
				float lerped = maxRopeSegmentLength / ropeSoftBody->m_links[i].m_rl;

				ropeSoftBody->m_nodes[i + 1].m_x = lerp(ropeSoftBody->m_nodes[i].m_x,
					ropeSoftBody->m_nodes[i + 1].m_x, maxRopeSegmentLength
					/ ropeSoftBody->m_links[i].m_rl);
				ropeSoftBody->m_nodes[i + 1].m_q = lerp(ropeSoftBody->m_nodes[i].m_q,
					ropeSoftBody->m_nodes[i + 1].m_q, maxRopeSegmentLength
					/ ropeSoftBody->m_links[i].m_rl);
				ropeSoftBody->m_links[i].m_rl = maxRopeSegmentLength;
			}
		}
	}

	btScalar afterChainAdjustmentLength = getSegmentLength(idxFirstLink, idxLastLink);
	btScalar deltaLength = length - afterChainAdjustmentLength;

	btSoftBody::Link &lastLink = ropeSoftBody->m_links[idxLastLink];

	btSoftBody::Node &beforeLastNode = *(lastLink.m_n[0]);

	btSoftBody::Node &lastNode = *(lastLink.m_n[1]);

	btScalar nodeDistance = lastLink.m_rl;

	btScalar scale = 1 + deltaLength / nodeDistance;

	if (nodeDistance + deltaLength <= maxRopeSegmentLength && nodeDistance
		+ deltaLength > 0)
	{ /// change link length
		lastNode.m_x = lerp(beforeLastNode.m_x, lastNode.m_x, scale);
		lastNode.m_q = lerp(beforeLastNode.m_q, lastNode.m_q, scale);
		lastLink.m_rl += deltaLength;
	}
	else if (nodeDistance + deltaLength > maxRopeSegmentLength)
	{ /// Need to add node
		if (nodeDistance + deltaLength > 2 * maxRopeSegmentLength)
		{
			std::cerr
				<< " *** WARNING *** The kite length is growing too fast. This could lead to trouble!"
				<< std::endl;
		}
		btVector3 m_x = lerp(beforeLastNode.m_x, lastNode.m_x, scale);
		// Fix the previous last node
		lastNode.m_x = lerp(beforeLastNode.m_x, lastNode.m_x,
			maxRopeSegmentLength / nodeDistance);
		lastNode.m_q = lerp(beforeLastNode.m_q, lastNode.m_q,
			maxRopeSegmentLength / nodeDistance);
		lastLink.m_rl = maxRopeSegmentLength;
		// Add new node
		ropeSoftBody->appendNode(m_x, 1 / lastNode.m_im);
//		ropeSoftBody->m_springAnchor->setSpringStartNode(ropeSoftBody->m_nodes.size() - 1);
//		ropeSoftBody->m_springAnchor->setSpringEndNode(ropeSoftBody->m_nodes.size() - 1);
		ropeSoftBody->appendLink(ropeSoftBody->m_nodes.size() - 2, ropeSoftBody->m_nodes.size()
			- 1);
		ropeSoftBody->m_links[ropeSoftBody->m_links.size() - 1].m_rl = nodeDistance
			+ deltaLength - maxRopeSegmentLength;
	}
	/// Need to remove a node
	else
	{
		if (idxFirstLink != idxLastLink)
		{
//			ropeSoftBody->m_springAnchor->setSpringEndNode(ropeSoftBody->m_nodes.size() - 2);
//			ropeSoftBody->m_springAnchor->setSpringStartNode(ropeSoftBody->m_nodes.size() - 2);

			ropeSoftBody->m_ndbvt.remove(lastNode.m_leaf);

			for (int i = idxLastLink + 1; i < ropeSoftBody->m_nodes.size() - 1; i++)
			{
				ropeSoftBody->m_nodes.swap(i, i + 1);
			}
			ropeSoftBody->m_nodes.pop_back();
			for (int i = idxLastLink; i < ropeSoftBody->m_links.size() - 1; i++)
			{
				ropeSoftBody->m_links.swap(i, i + 1);
			}
			ropeSoftBody->m_links.pop_back();

			std::cout << "Recursive call" << std::endl;
			setSegmentLength(idxFirstLink, idxLastLink - 1, length);
		}
		else
		{
			std::cerr
				<< " *** WARNING *** Attempt to have a zero-length or smaller kite. Request ignored!"
				<< std::endl;
		}
	}

	ropeSoftBody->updateNormals();
	ropeSoftBody->updateBounds();
	ropeSoftBody->updateConstants();
	ropeSoftBody->setTotalMass(mass);

	float finalSegmentLength = getSegmentLength(idxFirstLink, idxLastLink);
	std::cout << "initialSegmentLength = " << initialSegmentLength  << " finalSegmentLength = " << finalSegmentLength << std::endl;
}

btScalar PoRopeSoftBody::getSegmentLength(const int& idxFirstLink, const int& idxLastLink)
{
	btScalar length = 0.0;

	if (idxFirstLink >= 0 && idxFirstLink < ropeSoftBody->m_links.size()
		&& idxLastLink >= 0 && idxLastLink < ropeSoftBody->m_links.size()
		&& idxFirstLink <= idxLastLink)
	{
		for (int i = idxFirstLink; i <= idxLastLink; i++)
		{
			length += ropeSoftBody->m_links[i].m_rl;
			//std::cout << "link " << i << " rest length = " << ropeSoftBody->m_links[i].m_rl << std::endl;
		}
	}
	return length;
}