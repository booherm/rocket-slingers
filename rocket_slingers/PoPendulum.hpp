#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void doRenderUpdate();
	void inputEventCallback(InputEvent inputEvent);

private:
	void initGeometry();
	const float scalerToMeter = 3.0f;
	unsigned int clickCount = 0;
};

#endif
