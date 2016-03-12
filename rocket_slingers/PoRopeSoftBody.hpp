#ifndef POROPESOFTBODY_HPP
#define POROPESOFTBODY_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"

class PoRopeSoftBody : public PhysicalObject {
public:

	PoRopeSoftBody(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	void gameEventCallback(const Event& eventObj);
	~PoRopeSoftBody();

private:

	PoGuy* player;
	btSoftBody* ropeSoftBody;
	PhysicalMass* bob;

	glm::vec3 ropeStartPosition;
	glm::vec3 ropeEndPosition;
	unsigned int maxRopeSegments;
	float maxRopeSegmentLength;
	unsigned int initalRopeSegments;

	void initShaders();
	void initGeometry();
	void initPhysics();
	void setSegmentLength(const int& idxFirstLink, const int& idxLastLink, const btScalar& length);
	btScalar getSegmentLength(const int& idxFirstLink, const int& idxLastLink);
};

#endif
