#pragma once

#ifndef _Host_h
#define _Host_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <SensorBase.h>
class Host
{
public:
	//tell the host how many sensor i have
	Host(int totalSensorNum);
	~Host();
	//this adds a sensor to the host, channel will be created at last,
	//the host has list of channels, the order of the channels will be the order you add the sensor and channels in the sensor
	//sensor1-ch1,sensor1-ch2,sensor2-ch1....
	//returns the sensor added, <0 for errors
	int AddSensor(SensorBase* sensor);
	//start the host, this does one thing, create the channel value array
	//if the sensor added is not equal to the total sensor num error will returned, you have to stop here
	int Start();
	int Start(float * initChanelValues);
	//update all the sensors, return 0 for ok
	int Update();

	//if the host has started succesfully
	bool GetIsHostValid();


	//all the channel values
	float* ChannelSamples;
	//all the channels that all the sensors have
	int TotalChannelNum = 0;

private:

	//an arrayr for all the sensor pointers
	SensorBase** sensors;
	int totalSensorNum=0;
	//during adding sensor, track how many sensors are added
	int sensorAddedNum=0;


	bool isHostValid = false;
};

#endif




