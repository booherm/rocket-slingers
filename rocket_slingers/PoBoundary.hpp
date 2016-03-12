#ifndef POBOUNDARY_HPP
#define POBOUNDARY_HPP

#include "PhysicalObject.hpp"

class PoBoundary : public PhysicalObject {
public:
	PoBoundary(const std::string& objectId, GameState* gameState, const glm::vec3& scalers, const glm::vec3& position);
	void doPhysicalUpdate();
	void render();
	~PoBoundary();

private:

	PhysicalMass* physicalMass;
	glm::vec3 position;
	glm::vec3 scalers;

	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
