#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void doRenderUpdate();
	void inputEventCallback(const SDL_Event& inputEvent);

private:
	void doPhysicalUpdate();
	void initGeometry();
	unsigned int clickCount = 0;
};

#endif
