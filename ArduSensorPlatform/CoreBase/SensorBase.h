/*
 Name:		SensorBase.h
 Created:	8/29/2017 6:12:12 PM
 Author:	zhengwei
 Editor:	http://www.visualmicro.com
*/
#include<Host.h>
#ifndef _SensorBase_h
#define _SensorBase_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SensorBase
{
public:
	SensorBase();
	~SensorBase();

	//get the total channel number of this sensor
	virtual int GetChnanelNum() = 0;

	//update the sensor samples here, do you hardwork here not read, fetch data from sensor hardware
	//the return value is the error code, 0 for ok, none 0 for errors
	virtual int Update() = 0;

	//read the sample from a channel, note that the return value is float, if your sensor has complex readings use multiple channels
	virtual float Read(int chanelNo) = 0;

	//write the sample to a channel, the async flag indecate if you want to update the value imediatly or save if in sample and later updated.
	virtual int Write(int channelNo, float value, bool isAsync = false) = 0;

};

#endif

