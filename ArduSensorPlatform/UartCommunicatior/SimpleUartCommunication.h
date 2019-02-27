#pragma once



#ifndef _SimpleUartCommunication_h
#define _SimpleUartCommunication_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <CommunicationBase.h>

class SimpleUartCommunication:public CommunicationBase
{
public:
	SimpleUartCommunication();
	~SimpleUartCommunication();

	// Inherited via CommunicationBase
	virtual int HandleCommunication(Host* host) override;
};

#endif

