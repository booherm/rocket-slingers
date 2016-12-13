#ifndef POSPINANCHOR_HPP
#define POSPINANCHOR_HPP

#include "PhysicalObject.hpp"

class PoSpinAnchor : public PhysicalObject {
public:
	PoSpinAnchor(const std::string& objectId, GameState* gameState, const glm::vec3& scalers, const glm::vec3& position);
	void render();
	~PoSpinAnchor();

private:

	b2Body* rigidBody;
	glm::vec3 position;
	glm::vec3 scalers;

	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
