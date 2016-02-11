#include "InputQueue.hpp"
#include <iostream>

InputQueue::InputQueue(GameState* gameState) {
	this->gameState = gameState;
	inputQueue.resize(inputEventRetention);
	inputQueueNextSlot = 0;

	screenToWorldCoordinateScalerX = (gameState->worldViewportScaler * gameState->aspectRatio) / gameState->resolutionWidth;
	screenToWorldCoordinateScalerY = gameState->worldViewportScaler / gameState->resolutionHeight;
}

void InputQueue::addEvent(int glfwKey, int glfwKeyState, unsigned char glfwKeyModifier, double xPosition, double yPosition) {
	const unsigned char binary1 = (unsigned char) 1;
	const unsigned char binary2 = (unsigned char) 2;
	const unsigned char binary4 = (unsigned char) 4;
	const unsigned char binary8 = (unsigned char) 8;

	InputEvent e;
	e.eventKey = InputEvent::translateGlfwInputEventKey(glfwKey);
	e.eventKeyState = InputEvent::translateGlfwInputEventKeyState(glfwKeyState);
	e.modifierShift = (binary1 & glfwKeyModifier) == binary1;
	e.modifierControl = (binary2 & glfwKeyModifier) == binary2;
	e.modifierAlt = (binary4 & glfwKeyModifier) == binary4;
	e.modifierSuper = (binary8 & glfwKeyModifier) == binary8;
	e.xScreenCoordinate = xPosition;
	e.yScreenCoordinate = yPosition;

	e.xWorldCoordinate = (float)xPosition * screenToWorldCoordinateScalerX;
	e.yWorldCoordinate = gameState->worldViewportScaler - ((float) yPosition * screenToWorldCoordinateScalerY);

	enqueueEvent(e);
}

void InputQueue::addMouseMovementEvent(double xPosition, double yPosition) {
	InputEvent e;
	e.eventKey = InputEvent::IEK_MOUSE_MOVE;
	e.eventKeyState = InputEvent::IEKS_NO_STATE;
	e.modifierShift = false;
	e.modifierControl = false;
	e.modifierAlt = false;
	e.modifierSuper = false;
	e.xScreenCoordinate = xPosition;
	e.yScreenCoordinate = yPosition;

	e.xWorldCoordinate = (float)xPosition * screenToWorldCoordinateScalerX;
	e.yWorldCoordinate = gameState->worldViewportScaler - ((float) yPosition * screenToWorldCoordinateScalerY);

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
