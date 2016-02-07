#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void updatePhysicalState();
	void render();
	void inputEventCallback(InputEvent inputEvent);
	~PoPendulum();

private:
	void initGeometry();
	void initShaders();

	const unsigned int pendulumCount = 4;
	unsigned int clickCount = 0;
	GLfloat xInc;
	GLfloat yInc;
};

#endif
