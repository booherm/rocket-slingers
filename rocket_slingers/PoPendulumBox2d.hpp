#ifndef POPENDULUMBOX2D_HPP
#define POPENDULUMBOX2D_HPP

#include "PhysicalObject.hpp"

class PoPendulumBox2d : public PhysicalObject {
public:
	PoPendulumBox2d(const std::string& objectId, GameState* gameState);
	void render();
	~PoPendulumBox2d();

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
