#pragma once
#include <SensorBase.h>
#include <Adafruit_MAX31855.h>

#ifndef _MAX31855_ThermoSensor_h
#define _MAX31855_ThermoSensor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class MAX31855_ThermoSensor :public SensorBase
{
public:
	MAX31855_ThermoSensor(int8_t _sclk, int8_t _cs, int8_t _miso);
	~MAX31855_ThermoSensor();

	// Inherited via SensorBase
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAsync = false) override;

private:
	float sample[1];
	int8_t sclk, miso, cs;
	
};


#endif

