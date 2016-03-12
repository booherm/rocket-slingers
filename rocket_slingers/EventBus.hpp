#ifndef EVENTBUS_HPP
#define EVENTBUS_HPP

#include <map>
#include <vector>
#include "EventListener.hpp"
#include "GameState.hpp"

class GameState;

class EventBus {

public:
	EventBus(GameState* gameState);
	//void subscribeToMouseMotionEvent(EventListener* subscribingObject);
	void subscribeToKeyboardEvent(unsigned int keyState, unsigned int key, EventListener* subscribingObject);
	void subscribeToMouseButtonEvent(unsigned int buttonState, unsigned int buttonNumber, EventListener* subscribingObject);
	void subscribeToGameEvent(Event::GameEvent gameEvent, EventListener* subscribingObject);
	void postEvent(const Event& eventObj);
	void getMousePosition(float &x, float &y);
	bool processInput();

private:

	typedef std::pair<unsigned int, unsigned int> KeyboardSubscriptionKey;
	typedef std::pair<unsigned int, unsigned int> MouseButtonSubscriptionKey;

	GameState* gameState;
	std::map<unsigned int, Event::EventType> sdlEventTypeMapping;
	float screenToWorldCoordinateScalerX;
	float screenToWorldCoordinateScalerY;

	//std::vector<EventListener*> mouseMotionSubscriptions;
	std::multimap<KeyboardSubscriptionKey, EventListener*> keyboardSubscriptions;
	std::multimap<MouseButtonSubscriptionKey, EventListener*> mouseButtonSubscriptions;
	std::multimap<Event::GameEvent, EventListener*> gameEventSubscriptions;
	//std::multimap<MouseWheelSubscriptionKey, EventListener*> mouseWheelSubscriptions;

	void callSubscribers(const Event& eventObj);
};

#endif
