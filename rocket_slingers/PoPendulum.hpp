#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(const std::string& objectId, GameState* gameState);
	void render();
	~PoPendulum();

private:

	float sizeScaler = 5.0f;
	glm::vec3 modelOriginOffset;

	glm::vec3 initialPosition;
	void initShaders();
	void initGeometry();
	void initPhysics();
	void doPhysicalUpdate();

	b2Body* rigidBody;
	b2RevoluteJoint* revJoint;

};

#endif
