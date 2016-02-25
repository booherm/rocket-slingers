#ifndef POAXES_HPP
#define POAXES_HPP

#include "PhysicalObject.hpp"

class PoAxes : public PhysicalObject {
public:
	PoAxes(GameState* gameState);

private:
	const float xMin = 0.0f;
	const float xMax = 20.0f * gameState->aspectRatio;
	const float yMin = 0.0f;
	const float yMax = 20.0f;
	const float zMin = 0.0f;
	const float zMax = 0.0f;

	const float xMinorTickInterval = 1.0f;
	const float xMajorTickInterval = 5.0f;
	const float yMinorTickInterval = 1.0f;
	const float yMajorTickInterval = 5.0f;
	const float zMinorTickInterval = 1.0f;
	const float zMajorTickInterval = 5.0f;

	void initGeometry();
	void initRenderData();
	void pushAxisTransform(glm::mat4 model);
};

#endif
