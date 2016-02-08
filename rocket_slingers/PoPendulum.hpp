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

	unsigned int pendulumCount;
	const unsigned int maxPendulumCount = 100;
	unsigned int clickCount = 0;
	std::vector<glm::vec3> pendulumOrigins;
	std::vector<float> pendulumInitialTimes;
};

#endif
