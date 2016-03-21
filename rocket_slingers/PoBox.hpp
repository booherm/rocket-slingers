#ifndef POBOX_HPP
#define POBOX_HPP

#include "PhysicalObject.hpp"

class PoBox : public PhysicalObject {
public:
	PoBox(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	~PoBox();

private:

//	PhysicalMass* physicalMass;
	glm::vec3 position;

	float xScaler = 10.0f;
	float yScaler = 3.0f;

	b2Body* groundBody;
	b2Body* boxBody;
	b2Body* circleBody;
	
	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
