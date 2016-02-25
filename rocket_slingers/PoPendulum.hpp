#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void doRenderUpdate();
	void sdlInputEventCallback(const Event& eventObj);
	~PoPendulum();

private:

	float sizeScaler = 5.0f;

	glm::vec3 initialPosition;
	void doPhysicalUpdate();
	void initGeometry();
	void initPhysics();

	PhysicalMassMultiBody* multiBody;
};

#endif
