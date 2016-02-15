#ifndef EVENTLISTENER_HPP
#define EVENTLISTENER_HPP

#include "Event.hpp"

class EventListener
{
public:
	virtual void sdlInputEventCallback(const Event& eventObj);
	virtual void gameEventCallback(const Event& eventObj);
};

#endif
