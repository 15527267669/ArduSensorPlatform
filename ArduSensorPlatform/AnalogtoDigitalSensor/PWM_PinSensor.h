#pragma once
#ifndef  _PWM_PinSensor_h
#define _PWM_PinSensor_h
#include <SensorBase.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif




class PWM_PinSensor :public SensorBase
{
public:
	PWM_PinSensor(uint8_t _pwmPIN,  uint8_t _dirPIN);
	~PWM_PinSensor();

	// Inherited via SensorBase
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAsync = false) override;

	void Handle(float sample);//Core function:convert input and give output siginal to H bridge
private:
	float sample[1];
	int pwm_maker(float val);//convert input into pwm value
	void identify(float val);//distinguish input if "+" or "-" or "0"							
	void setPIN(); //set up PIN
	uint8_t pwmPIN, ssrPIN, dirPIN;
	const int INPUT_VALUE_MIN = 0;
	const int INPUT_VALUE_MAX = 10;
	const int OUTPUT_VALUE_MIN = 0;
	const int OUTPUT_VALUE_MAX = 229;//90%¡Á255
};


#endif