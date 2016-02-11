#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"

class PoGuy : public PhysicalObject {
public:
	PoGuy(GameState* gameState);
	void updatePhysicalState();
	void inputEventCallback(InputEvent inputEvent);

private:

	void initGeometry();
};

#endif
