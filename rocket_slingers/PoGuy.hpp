#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"
#include <map>

class PoGuy : public PhysicalObject {
public:
	PoGuy(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void sdlInputEventCallback(const Event& eventObj);
	~PoGuy();

private:

	const float scalerToMeter = 0.03936f;
	glm::vec3 initialPosition;
	PhysicalMass* physicalMass;

	bool rocketOn = false;
	void initGeometry();
	void initPhysics();
	void initEventSubsriptions();
	unsigned int soundEffectInstanceId;
	
	unsigned int keyDownCount;
	std::map<int, bool> keyStates;
};

#endif
