#ifndef POROPE_HPP
#define POROPE_HPP

#include "PhysicalObject.hpp"

class PoRope : public PhysicalObject {
public:
	PoRope(GameState* gameState);
	void updatePhysicalState();
	void render();
	void inputEventCallback(InputEvent inputEvent);
	~PoRope();

private:
	const unsigned int maxRopeCount = 100;
	const unsigned int ropeSegmentCount = 6;
	std::vector<glm::vec2> ropeOriginPoints;
	std::vector<glm::vec2> ropeTerminationPoints;
	unsigned int ropeCount = 0;
	void initGeometry();
	void initShaders();
};

#endif
