#ifndef POROPETARGET_HPP
#define POROPETARGET_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"

class PoRopeTarget : public PhysicalObject {
public:
	PoRopeTarget(const std::string& objectId, GameState* gameState);
	void render();
	void setRopeMaxLength(float maxLength);
	bool targetAquired;
	b2Vec2 playerPosition;
	b2Vec2 targetPosition;
	b2Body* targetRigidBody;
	b2Vec2 mousePosition;

private:

	float ropeMaxLength;
	void initShaders();
	void initGeometry();
	void doPhysicalUpdate();
};

#endif
