#ifndef INPUTQUEUE_HPP
#define INPUTQUEUE_HPP

#include <map>
#include <vector>
#include "InputEvent.hpp"
#include "EventListener.hpp"
#include "GameState.hpp"

class GameState;

class InputQueue {

public:
	InputQueue(GameState* gameState);
	bool processInput();
	void subscribeToInputEvent(
		InputEvent::InputEventKey inputEventKey,
		InputEvent::InputEventKeyState inputEventKeyState,
		EventListener* subscribingObject
	);
	~InputQueue();

private:

	void addSdlEvent(const SDL_Event& sdlEvent);

	GameState* gameState;
	typedef std::pair<InputEvent::InputEventKey, InputEvent::InputEventKeyState> subscriptionKey;
	const unsigned int inputEventRetention = 10;
	std::vector<InputEvent> inputQueue;
	unsigned int inputQueueNextSlot;
	std::multimap<subscriptionKey, EventListener*> eventSubscriptions;
	void enqueueEvent(InputEvent inputEvent);
	float screenToWorldCoordinateScalerX;
	float screenToWorldCoordinateScalerY;
};

#endif
