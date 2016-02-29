#ifndef POBACKGROUND_HPP
#define POBACKGROUND_HPP

#include "PhysicalObject.hpp"

class PoBackground : public PhysicalObject {
public:
	PoBackground(const std::string& objectId, GameState* gameState);
	void render();

private:

	float sizeScaler;
	unsigned int parallaxLayers;
	void initShaders();
	void initGeometry();

};

#endif
