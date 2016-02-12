#ifndef POAXES_HPP
#define POAXES_HPP

#include "PhysicalObject.hpp"

class PoAxes : public PhysicalObject {
public:
	PoAxes(GameState* gameState);
	void inputEventCallback(const SDL_Event& inputEvent);

private:
	const float xMin = 0.0f;
	const float xMax = 20.0f * gameState->aspectRatio;
	const float yMin = 0.0f;
	const float yMax = 20.0f;
	const float zMin = 0.0f;
	const float zMax = 0.0f;
	bool initialized = false;

	void initGeometry();
	void initRenderData();
	void pushAxisTransform(glm::mat4 model);
};

#endif
