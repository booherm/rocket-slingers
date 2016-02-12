#ifndef INPUTQUEUE_HPP
#define INPUTQUEUE_HPP

#include <map>
#include <vector>
#include "EventListener.hpp"
#include "GameState.hpp"

class GameState;

class InputQueue {

public:
	InputQueue(GameState* gameState);
	void subscribeToKeyboardEvent(unsigned int keyState, unsigned int key, EventListener* subscribingObject);
	void subscribeToMouseButtonEvent(unsigned int buttonState, unsigned int buttonNumber, EventListener* subscribingObject);
	bool processInput();

	float eventWorldCoordinateX;
	float eventWorldCoordinateY;

private:

	typedef std::pair<unsigned int, unsigned int> KeyboardSubscriptionKey;
	typedef std::pair<unsigned int, unsigned int> MouseButtonSubscriptionKey;

	GameState* gameState;
	float screenToWorldCoordinateScalerX;
	float screenToWorldCoordinateScalerY;

	std::multimap<KeyboardSubscriptionKey, EventListener*> keyboardSubscriptions;
	std::multimap<MouseButtonSubscriptionKey, EventListener*> mouseButtonSubscriptions;
	//std::multimap<MouseMotionSubscriptionKey, EventListener*> mouseMotionSubscriptions;
	//std::multimap<MouseWheelSubscriptionKey, EventListener*> mouseWheelSubscriptions;

	void callSubscribers(const SDL_Event& inputEvent);
};

#endif
