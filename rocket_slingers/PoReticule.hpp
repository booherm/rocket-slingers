#ifndef PORETICULE_HPP
#define PORETICULE_HPP

#include "PhysicalObject.hpp"

class PoReticule : public PhysicalObject {
public:
	PoReticule(const std::string& objectId, GameState* gameState);
	void gameEventCallback(const Event& eventObj);
	void render();

private:
	
	glm::vec3 worldPosition;
	void initShaders();
	void initGeometry();
};

#endif
