#ifndef EVENTLISTENER_HPP
#define EVENTLISTENER_HPP

#include "InputEvent.hpp"

class EventListener
{
public:
	virtual void inputEventCallback(InputEvent inputEvent);
};

#endif
