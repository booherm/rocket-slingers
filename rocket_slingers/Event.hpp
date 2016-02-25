#ifndef EVENT_HPP
#define EVENT_HPP

#include <sdl.h>

class EventListener;

class Event {
public:

	enum EventType {
		NULL_EVENT,
		SDL_KEYBOARD,
		SDL_MOUSE_MOVEMENT,
		SDL_MOUSE_BUTTON,
		GAME_EVENT
	};

	enum GameEvent {
		THROW_ROPE,
		RELEASE_ROPE,
		MOVE_RETICULE
	};

	EventType eventType;
	SDL_Event* sdlInputEvent;
	GameEvent gameEvent;
	float eventWorldCoordinateX;
	float eventWorldCoordinateY;
	EventListener* eventPoster;
};

#endif
