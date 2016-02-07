#ifndef POAXES_HPP
#define POAXES_HPP

#include "PhysicalObject.hpp"

class PoAxes : public PhysicalObject {
public:
	PoAxes(GameState* gameState);
	void updatePhysicalState();
	void render();
	void inputEventCallback(InputEvent inputEvent);
	~PoAxes();

private:
	const float xMin = -1.0f;
	const float xMax = 1.0f;
	const float yMin = -1.0f;
	const float yMax = 1.0f;
	const float zMin = -1.0f;
	const float zMax = 1.0f;
	const unsigned int minorTicks = 100;
	const unsigned int majorTicks = 10;
	unsigned int axesLinesCount;

	void initGeometry();
	void initShaders();
	void pushAxisTransform(glm::mat4 model);
};

#endif
