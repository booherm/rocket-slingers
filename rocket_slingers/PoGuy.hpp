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

private:

	const float scalerToMeter = 0.03936f;
	bool rocketOn = false;
	void initGeometry();
	const float airFrictionConstant = 0.5f;
	unsigned int soundEffectInstanceId;
	
	unsigned int keyDownCount;
	std::map<int, bool> keyStates;
};

#endif
