#include "InputQueue.hpp"
#include <iostream>

InputQueue::InputQueue(GameState* gameState) {
	this->gameState = gameState;
	inputQueue.resize(inputEventRetention);
	inputQueueNextSlot = 0;

	screenToWorldCoordinateScalerX = (gameState->worldViewportScaler * gameState->aspectRatio) / gameState->resolutionWidth;
	screenToWorldCoordinateScalerY = gameState->worldViewportScaler / gameState->resolutionHeight;
}

bool InputQueue::processInput() {

//	unsigned int eventCount = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			return false;
		}
		else {
			addSdlEvent(event);
	//		eventCount++;
		}
	}

//	if(eventCount > 0)
	//	std::cout << "processed " << eventCount << " events" << std::endl;

	return true;
}

void InputQueue::addSdlEvent(const SDL_Event& sdlEvent) {

	InputEvent e;

	if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
		e.eventKey = InputEvent::translateSdlInputEventKey(sdlEvent.key.keysym.sym);
		e.eventKeyState = InputEvent::translateSdlInputEventKeyState(sdlEvent.key.type);
	}
	else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) {
		if(sdlEvent.button.button == 1)
			e.eventKey = InputEvent::IEK_MOUSE_BUTTON_1;
		else if(sdlEvent.button.button == 2)
			e.eventKey = InputEvent::IEK_MOUSE_BUTTON_2;
		else if (sdlEvent.button.button == 3)
			e.eventKey = InputEvent::IEK_MOUSE_BUTTON_3;

		if(sdlEvent.type == SDL_MOUSEBUTTONDOWN)
			e.eventKeyState = InputEvent::IEKS_KEYDOWN;
		else
			e.eventKeyState = InputEvent::IEKS_KEYUP;
	}
	else if (sdlEvent.type == SDL_MOUSEMOTION) {
		e.eventKey = InputEvent::IEK_MOUSE_MOVE;
		e.eventKeyState = InputEvent::IEKS_NO_STATE;
	}
	e.xScreenCoordinate = sdlEvent.motion.x;
	e.yScreenCoordinate = sdlEvent.motion.y;
	e.xWorldCoordinate = (float) e.xScreenCoordinate * screenToWorldCoordinateScalerX;
	e.yWorldCoordinate = gameState->worldViewportScaler - ((float)e.yScreenCoordinate * screenToWorldCoordinateScalerY);

	enqueueEvent(e);
}

void InputQueue::enqueueEvent(InputEvent inputEvent) {
	inputQueue[inputQueueNextSlot++] = inputEvent;
	if (inputQueueNextSlot == inputEventRetention)
		inputQueueNextSlot = 0;

	// call subscribers to this event
	subscriptionKey key(inputEvent.eventKey, inputEvent.eventKeyState);
	std::pair<std::multimap<subscriptionKey, EventListener*>::iterator, std::multimap<subscriptionKey, EventListener*>::iterator> subscribersRange;
	subscribersRange = eventSubscriptions.equal_range(key);
	for (std::multimap<subscriptionKey, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
		EventListener* callbackObject = &*(it->second);
		callbackObject->inputEventCallback(inputEvent);
	}
}

void InputQueue::subscribeToInputEvent(
	InputEvent::InputEventKey inputEventKey,
	InputEvent::InputEventKeyState inputEventKeyState,
	EventListener* subscribingObject
) {
	subscriptionKey key(inputEventKey, inputEventKeyState);
	eventSubscriptions.insert(std::pair<subscriptionKey, EventListener*>(key, subscribingObject));
}

InputQueue::~InputQueue() {
}
