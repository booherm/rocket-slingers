#ifndef PHYSICALOBJECT_HPP
#define PHYSICALOBJECT_HPP

#include <iostream>
#include <vector>
#include "EventListener.hpp"
#include "GameState.hpp"
#include "Utilities.hpp"
#include "PhysicalMass.hpp"
#include "PhysicalMassMultiBody.hpp"
#include "RenderableObject.hpp"

class EventBus;
class GameState;

class PhysicalObject : public EventListener, public RenderableObject {
public:
	PhysicalObject(const std::string& objectId, GameState* gameState);
	virtual ~PhysicalObject();

	virtual void sdlInputEventCallback(const Event& eventObj);
	virtual void gameEventCallback(const Event& eventObj);
	std::string objectId;

	// physics
	void updatePhysicalState();

protected:

	GameState* gameState;

	// physics
	virtual void doPhysicalUpdate();
	bool shouldDoPhysicalUpdate;
};

#endif
