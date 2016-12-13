#ifndef POBOUNDARY2_HPP
#define POBOUNDARY2_HPP

#include "PhysicalObject.hpp"

class PoBoundary2 : public PhysicalObject {
public:
	PoBoundary2(const std::string& objectId, GameState* gameState, const glm::vec3& scalers, const glm::vec3& position);
	void render();
	~PoBoundary2();

private:

	b2Body* rigidBody;
	glm::vec3 position;
	glm::vec3 scalers;

	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
