#ifndef POAXES_HPP
#define POAXES_HPP

#include "PhysicalObject.hpp"

class PoAxes : public PhysicalObject {
public:
	PoAxes(GameState* gameState);
	void updatePhysicalState();
	void inputEventCallback(InputEvent inputEvent);

private:
	const float xMin = -1.0f;
	const float xMax = 1.0f;
	const float yMin = -1.0f;
	const float yMax = 1.0f;
	const float zMin = -1.0f;
	const float zMax = 1.0f;

	void initGeometry();
	void pushAxisTransform(glm::mat4 model);
};

#endif
