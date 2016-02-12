#ifndef EVENTLISTENER_HPP
#define EVENTLISTENER_HPP

#include <SDL.h>

class EventListener
{
public:
	virtual void inputEventCallback(const SDL_Event& inputEvent);
};

#endif
