#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"
#include <map>

class PoGuy : public PhysicalObject {
public:
	PoGuy(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void inputEventCallback(const SDL_Event& inputEvent);

private:

	const float scalerToMeter = 0.03936f;
	bool rocketOn = false;
	void initGeometry();
	const float airFrictionConstant = 0.1f;
	unsigned int soundEffectInstanceId;
	
	std::map<int, bool> keyStates;

};

#endif
