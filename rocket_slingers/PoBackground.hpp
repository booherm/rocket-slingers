#ifndef POBACKGROUND_HPP
#define POBACKGROUND_HPP

#include "PhysicalObject.hpp"

class PoBackground : public PhysicalObject {
public:
	PoBackground(const std::string& objectId, GameState* gameState);
	void gameEventCallback(const Event& eventObj);
	void render();

private:

	glm::vec3 cameraPosition;
	glm::mat4 projectionTransform;
	float sizeScaler;
	unsigned int parallaxLayers;
	void initShaders();
	void initGeometry();

};

#endif
