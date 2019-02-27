#include "MAX31855_ThermoSensor.h"

MAX31855_ThermoSensor::MAX31855_ThermoSensor(int8_t _sclk, int8_t _cs, int8_t _miso)
{
	sclk = _sclk;
	miso = _miso;
	cs = _cs;
}

MAX31855_ThermoSensor::~MAX31855_ThermoSensor()
{
}

int MAX31855_ThermoSensor::GetChnanelNum()
{
	return 1;
}

int MAX31855_ThermoSensor::Update()
{
	Adafruit_MAX31855 max31855(sclk, cs, miso);
	sample[0] =(float) max31855.readCelsius();
	return 0;
}

float MAX31855_ThermoSensor::Read(int chanelNo)
{
	//wee have only one channel so channelNo is ignored
	return sample[0];
}

int MAX31855_ThermoSensor::Write(int channelNo, float value, bool isAsync)
{
	return 0;
}
