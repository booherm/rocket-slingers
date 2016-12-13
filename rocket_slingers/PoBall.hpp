#ifndef POBALL_HPP
#define POBALL_HPP

#include "PhysicalObject.hpp"

class PoBall : public PhysicalObject {
public:
	PoBall(const std::string& objectId, GameState* gameState, float radius, const b2Vec2& position);
	void render();
	void doPhysicalUpdate();
	~PoBall();

private:

	b2Body* rigidBody;
	b2Vec2 position;
	float radius;

	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
