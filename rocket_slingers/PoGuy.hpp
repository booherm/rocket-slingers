#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"

class PoGuy : public PhysicalObject {
public:
	PoGuy(GameState* gameState);
	void updatePhysicalState();
	void render();
	void inputEventCallback(InputEvent inputEvent);
	~PoGuy();

private:
	unsigned int guyCount = 1;
	unsigned int guyTriangleCount;
	void initGeometry();
	void initShaders();
};

#endif
