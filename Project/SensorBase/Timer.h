#pragma once

#ifndef _Timer_h
#define _Timer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
class Timer
{
	//this class is used to time the loop of the sensor update
	//note this is just a example and base class, you needto extend it to your needs
	//sleep, powerdown and other power saving should 
public:
	Timer(long interval);
	~Timer();
	//test if now is time to do something
	virtual bool OnTime();
private:
	long time;
	long interval;
};


#endif
