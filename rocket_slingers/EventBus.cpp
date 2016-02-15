#include "EventBus.hpp"
#include <iostream>

EventBus::EventBus(GameState* gameState) {
	this->gameState = gameState;

	screenToWorldCoordinateScalerX = (gameState->worldViewportScaler * gameState->aspectRatio) / gameState->resolutionWidth;
	screenToWorldCoordinateScalerY = gameState->worldViewportScaler / gameState->resolutionHeight;

	sdlEventTypeMapping[SDL_FIRSTEVENT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_QUIT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_TERMINATING] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_LOWMEMORY] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_WILLENTERBACKGROUND] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_DIDENTERBACKGROUND] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_WILLENTERFOREGROUND] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_APP_DIDENTERFOREGROUND] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_WINDOWEVENT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_SYSWMEVENT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_KEYDOWN] = Event::EventType::SDL_KEYBOARD;
	sdlEventTypeMapping[SDL_KEYUP] = Event::EventType::SDL_KEYBOARD;
	sdlEventTypeMapping[SDL_TEXTEDITING] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_TEXTINPUT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_KEYMAPCHANGED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_MOUSEMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_MOUSEBUTTONDOWN] = Event::EventType::SDL_MOUSE_BUTTON;
	sdlEventTypeMapping[SDL_MOUSEBUTTONUP] = Event::EventType::SDL_MOUSE_BUTTON;
	sdlEventTypeMapping[SDL_MOUSEWHEEL] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYAXISMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYBALLMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYHATMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYBUTTONDOWN] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYBUTTONUP] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYDEVICEADDED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_JOYDEVICEREMOVED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERAXISMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERBUTTONDOWN] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERBUTTONUP] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERDEVICEADDED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERDEVICEREMOVED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CONTROLLERDEVICEREMAPPED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_FINGERDOWN] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_FINGERUP] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_FINGERMOTION] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_DOLLARGESTURE] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_DOLLARRECORD] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_MULTIGESTURE] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_CLIPBOARDUPDATE] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_DROPFILE] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_AUDIODEVICEADDED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_AUDIODEVICEREMOVED] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_RENDER_TARGETS_RESET] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_RENDER_DEVICE_RESET] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_USEREVENT] = Event::EventType::NULL_EVENT;
	sdlEventTypeMapping[SDL_LASTEVENT] = Event::EventType::NULL_EVENT;

}

void EventBus::subscribeToKeyboardEvent(unsigned int keyState, unsigned int key, EventListener* subscribingObject) {
	KeyboardSubscriptionKey subscriptionKey(keyState, key);
	keyboardSubscriptions.insert(std::pair<KeyboardSubscriptionKey, EventListener*>(subscriptionKey, subscribingObject));
}

void EventBus::subscribeToMouseButtonEvent(unsigned int buttonState, unsigned int buttonNumber, EventListener* subscribingObject) {
	MouseButtonSubscriptionKey subscriptionKey(buttonState, buttonNumber);
	mouseButtonSubscriptions.insert(std::pair<MouseButtonSubscriptionKey, EventListener*>(subscriptionKey, subscribingObject));
}

void EventBus::subscribeToGameEvent(Event::GameEvent gameEvent, EventListener* subscribingObject) {
	gameEventSubscriptions.insert(std::pair<Event::GameEvent, EventListener*>(gameEvent, subscribingObject));
}

bool EventBus::processInput() {

	SDL_Event inputEvent;
	while (SDL_PollEvent(&inputEvent)) {
		if (inputEvent.type == SDL_KEYDOWN && inputEvent.key.keysym.sym == SDLK_ESCAPE) {
			return false;
		}
		else
		{
			Event e;
			e.eventType = sdlEventTypeMapping[inputEvent.type];
			if (e.eventType == Event::EventType::NULL_EVENT || inputEvent.key.repeat != 0)  // some input we don't care about, exit
				return true;

			e.sdlInputEvent = &inputEvent;
			e.eventPoster = nullptr;
			e.eventWorldCoordinateX = (float) inputEvent.button.x * screenToWorldCoordinateScalerX;
			e.eventWorldCoordinateY = gameState->worldViewportScaler - ((float) inputEvent.button.y * screenToWorldCoordinateScalerY);

			callSubscribers(e);
		}
	}

	return true;
}

// call subscribers to this event
void EventBus::callSubscribers(const Event& eventObj) {
	switch (eventObj.eventType) {

		// keyboard
		case Event::EventType::SDL_KEYBOARD:
		{
			KeyboardSubscriptionKey subscriptionKey(eventObj.sdlInputEvent->key.state, eventObj.sdlInputEvent->key.keysym.sym);
			std::pair<std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator,
				std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator> subscribersRange;
			subscribersRange = keyboardSubscriptions.equal_range(subscriptionKey);
			for (std::multimap<KeyboardSubscriptionKey, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
				EventListener* callbackObject = &*(it->second);
				callbackObject->sdlInputEventCallback(eventObj);
			}

			break;
		}

		// mouse button
		case Event::EventType::SDL_MOUSE_BUTTON:
		{
			MouseButtonSubscriptionKey subscriptionKey(eventObj.sdlInputEvent->button.state, eventObj.sdlInputEvent->button.button);
			std::pair<std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator,
				std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator> subscribersRange;
			subscribersRange = mouseButtonSubscriptions.equal_range(subscriptionKey);
			for (std::multimap<MouseButtonSubscriptionKey, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
				EventListener* callbackObject = &*(it->second);
				callbackObject->sdlInputEventCallback(eventObj);
			}
			break;
		}

		// game event
		case Event::EventType::GAME_EVENT:
		{
			std::pair<std::multimap<Event::GameEvent, EventListener*>::iterator,
				std::multimap<Event::GameEvent, EventListener*>::iterator> subscribersRange;
			subscribersRange = gameEventSubscriptions.equal_range(eventObj.gameEvent);
			for (std::multimap<Event::GameEvent, EventListener*>::iterator it = subscribersRange.first; it != subscribersRange.second; ++it) {
				EventListener* callbackObject = &*(it->second);
				callbackObject->gameEventCallback(eventObj);
			}
			break;
		}

	}
}

void EventBus::postEvent(const Event& eventObj) {
	callSubscribers(eventObj);
}
