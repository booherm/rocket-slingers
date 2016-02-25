#ifndef PORETICULE_HPP
#define PORETICULE_HPP

#include "PhysicalObject.hpp"

class PoReticule : public PhysicalObject {
public:
	PoReticule(GameState* gameState);
	void gameEventCallback(const Event& eventObj);

private:
	
	glm::vec3 worldPosition;
	void initGeometry();
	void initRenderData();
	void doRenderUpdate();
	void setupTransforms();
};

#endif
