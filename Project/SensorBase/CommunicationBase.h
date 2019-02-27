#pragma once


#ifndef _CommunicationBase_h
#define _CommunicationBase_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Host.h>

class CommunicationBase
{
public:
	CommunicationBase();
	~CommunicationBase();
	virtual int HandleCommunication(Host* host)=0;
};

#endif

