#include "Timer.h"



Timer::Timer(long interval)
{
	time = millis();
	this->interval = interval;
}


Timer::~Timer()
{
}

bool Timer::OnTime()
{
	auto now = millis();
	if ((now - time) > interval) //consider overflow here
	{
		time = now;
		return true;
	}
	return false;
}
