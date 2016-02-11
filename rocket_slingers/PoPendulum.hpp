#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void updatePhysicalState();
	void inputEventCallback(InputEvent inputEvent);

private:
	void initGeometry();

	unsigned int clickCount = 0;
};

#endif
