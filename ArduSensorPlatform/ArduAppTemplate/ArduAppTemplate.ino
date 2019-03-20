/*
 Name:		ArduAppTemplate.ino
 Created:	2019/2/28 9:01:32
 Author:	jtext103
*/
#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include <DigitalPin.h>
#include <ModbusRTUComm.h>
#include <Timer.h>

//static host declearation, but host created in setupSensor are recommanded
Host host(3);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;

Timer* sensorTimer;
Timer* commTimer;
float *initValues = NULL;

void setup() {
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(500);
	commTimer = new Timer(200);
	//if you donot need to init tha channels, comment the line below
	initAllChanells();

	//start, timer may need a start function
	host.Start(initValues);
	if (initValues != NULL)
	{
		delete[] initValues;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	//control loop
	if (sensorTimer->OnTime())
	{
		host.Update();
	}
	//it may not necessary to use another time base for comm
	if (commTimer->OnTime())
	{
		comm->HandleCommunication(&host);
	}

}

void setupSensors()
{
	//construct your sensors and add them to host
	SensorBase* sensor;
	sensor = new DigitalPin(4);
	host.AddSensor(sensor);
	sensor = new DigitalPin(5);
	host.AddSensor(sensor);
	sensor = new DigitalPin(6);
	host.AddSensor(sensor);
}

void initAllChanells()
{
	initValues = new float[5]();
	for (size_t i = 0; i < 5; i++)
	{
		initValues[i] = 0;
	}
}

void setupComm()
{
	//normally you have more things to do tosetup a proper communication
	//like port, speed, address and etc.
	comm = new ModbusRTUComm(&Serial, 9600, 2);
}
