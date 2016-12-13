#ifndef PHYSICALOBJECT_HPP
#define PHYSICALOBJECT_HPP

#include <iostream>
#include <vector>
#include "EventListener.hpp"
#include "GameState.hpp"
#include "Utilities.hpp"
#include "RenderableObject.hpp"
#include "WorldRayCaster.hpp"

class EventBus;
class GameState;

class PhysicalObject : public EventListener, public RenderableObject {
public:
	PhysicalObject(const std::string& objectId, GameState* gameState);
	virtual ~PhysicalObject();

	virtual void sdlInputEventCallback(const Event& eventObj);
	virtual void gameEventCallback(const Event& eventObj);
	virtual void processContactBegin(PhysicalObject* contactingObject, b2Contact* contact);
	virtual void processContactEnd(PhysicalObject* contactingObject, b2Contact* contact);
	std::string objectId;
	bool ropeAttachable;
	b2Vec2 ropeAttachmentPoint;

	// physics
	void updatePhysicalState();

protected:

	GameState* gameState;

	// physics
	virtual void doPhysicalUpdate();
	bool shouldDoPhysicalUpdate;
};

#endif
