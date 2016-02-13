#include "InputQueue.hpp"
#include <iostream>

InputQueue::InputQueue(GameState* gameState) {
	this->gameState = gameState;

	screenToWorldCoordinateScalerX = (gameState->worldViewportScaler * gameState->aspectRatio) / gameState->resolutionWidth;
	screenToWorldCoordinateScalerY = gameState->worldViewportScaler / gameState->resolutionHeight;
}

void InputQueue::subscribeToKeyboardEvent(unsigned int keyState, unsigned int key, EventListener* subscribingObject) {
	KeyboardSubscriptionKey subscriptionKey(keyState, key);
	keyboardSubscriptions.insert(std::pair<KeyboardSubscriptionKey, EventListener*>(subscriptionKey, subscribingObject));
}

void InputQueue::subscribeToMouseButtonEvent(unsigned int buttonState, unsigned int buttonNumber, EventListener* subscribingObject) {
	MouseButtonSubscriptionKey subscriptionKey(buttonState, buttonNumber);
	mouseButtonSubscriptions.insert(std::pair<MouseButtonSubscriptionKey, EventListener*>(subscriptionKey, subscribingObject));
}

bool InputQueue::processInput() {

	SDL_Event inputEvent;
	while (SDL_PollEvent(&inputEvent)) {
		if (inputEvent.type == SDL_KEYDOWN && inputEvent.key.keysym.sym == SDLK_ESCAPE) {
			return false;
		}
		else if(inputEvent.key.repeat == 0) {  // throw out key repeat events
			callSubscribers(inputEvent);
		}
	}

	return true;
}

// call subscribers to this event
void InputQueue::callSubscribers(const SDL_Event& inputEvent) {
	switch (inputEvent.type) {

		// keyboard
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			KeyboardSubscriptionKey subscriptionKey(inputEvent.key.state, inputEvent.key.keysym.sym);
			std::pair<std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator,
				std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator> subscribersRange;
			subscribersRange = keyboardSubscriptions.equal_range(subscriptionKey);
			for (std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
				EventListener* callbackObject = &*(it->second);
				callbackObject->inputEventCallback(inputEvent);
			}

			break;
		}

		// mouse button
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			// store event world mouse cooridnates in public variables, ugly to pass with event
			int xScreenCoordinate = inputEvent.button.x;
			int yScreenCoordinate = inputEvent.button.y;
			eventWorldCoordinateX = (float) xScreenCoordinate * screenToWorldCoordinateScalerX;
			eventWorldCoordinateY = gameState->worldViewportScaler - ((float) yScreenCoordinate * screenToWorldCoordinateScalerY);

			std::cout << "xScreenCoordinate = " << xScreenCoordinate << ", yScreenCoordinate = " << yScreenCoordinate << std::endl;
			MouseButtonSubscriptionKey subscriptionKey(inputEvent.button.state, inputEvent.button.button);
			std::pair<std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator,
				std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator> subscribersRange;
			subscribersRange = mouseButtonSubscriptions.equal_range(subscriptionKey);
			for (std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
				EventListener* callbackObject = &*(it->second);
				callbackObject->inputEventCallback(inputEvent);
			}
			break;
		}
	}
}


/*

*/
