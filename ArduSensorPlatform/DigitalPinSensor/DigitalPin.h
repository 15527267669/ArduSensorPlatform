#pragma once
#include <SensorBase.h>
#ifndef _DigitalPin_h
#define _DigitalPin_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class DigitalPin :public SensorBase
{
public:
	DigitalPin(int pinNo, bool isOutput = false);
	~DigitalPin();

	// Inherited via SensorBase
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAsync = false) override;

private:
	float sample[1];
	bool isOutput;
	int pinNo;
};


#endif



