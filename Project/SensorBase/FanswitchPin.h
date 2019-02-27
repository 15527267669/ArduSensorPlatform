#pragma once
#include <SensorBase.h>
#ifndef _FanswitchPin_h
#define _FanswitchPin_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class FanswitchPin:public SensorBase
{
public:
	FanswitchPin(int pinNo, Host*host,int hummchannelNumber);
	~FanswitchPin();
	// Inherited via SensorBase
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAsync = false) override;
	void Handle(float humm);//Core function:convert input and give output siginal to fan switch
private:
	void setPIN(); //set up PIN
	float sample[1];//the fanswitch pin value
	int pinNo;
	float humm;
	Host*host;
	int hummchannelNumber;
};


#endif // !_FanswitchPin_h
