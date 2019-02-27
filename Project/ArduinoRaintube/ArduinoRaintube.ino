/*
 Name:		ArduinoRaintube.ino
 Created:	3/31/2018 4:42:22 PM
 Author:	jtext103
*/

#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include<RaintubeSensor.h>
#include <ModbusRTUComm.h>
#include <Timer.h>

//static host declearation, but host created in setupSensor are recommanded
Host host(1);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;

Timer* sensorTimer;
Timer* commTimer;
float *initValues = NULL;
// the setup function runs once when you press reset or power the board
void setup() {
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(1000);
	commTimer = new Timer(500);
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

//set up sensor
void setupSensors()
{
	//construct our sensors and add them to host
	SensorBase* sensor;
	sensor = new RaintubeSensor(&Serial1, 9600, 2);
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

//set up modbus communication
void setupComm()
{
	//normally you have more things to do tosetup a proper communication
	//like port, speed, address and etc.
	//  comm = new ModbusRTUComm(&Serial, &Serial, 9600, 2);
	comm = new ModbusRTUComm(&Serial, 9600, 2);
}


//#include <SensorBase.h>
//#include <Host.h>
//#include<CommunicationBase.h>
//#include<TH10S_TempHum485.h>
//#include <ModbusRTUComm.h>
//#include <Timer.h>
//
////static host declearation, but host created in setupSensor are recommanded
//Host host(1);
////different as host setup, the communication is set up dunamically
//CommunicationBase* comm;
//
//Timer* sensorTimer;
//Timer* commTimer;
//float *initValues = NULL;
//// the setup function runs once when you press reset or power the board
//void setup() {
//	//setup all sensors
//	setupSensors();
//	//setup communications
//	setupComm();
//	//setup timers
//	sensorTimer = new Timer(1000);
//	commTimer = new Timer(500);
//	//if you donot need to init tha channels, comment the line below
//	initAllChanells();
//
//	//start, timer may need a start function
//	host.Start(initValues);
//	if (initValues != NULL)
//	{
//		delete[] initValues;
//	}
//}
//
//// the loop function runs over and over again until power down or reset
//void loop() {
//	//control loop
//	if (sensorTimer->OnTime())
//	{
//		host.Update();
//	}
//	//it may not necessary to use another time base for comm
//	if (commTimer->OnTime())
//	{
//		comm->HandleCommunication(&host);
//	}
//}
//
////set up sensor
//void setupSensors()
//{
//	//construct our sensors and add them to host
//	SensorBase* sensor;
//	sensor = new TH10S_TempHum485(&Serial1, 9600, 1);
//	host.AddSensor(sensor);
//}
//
//void initAllChanells()
//{
//	initValues = new float[5]();
//	for (size_t i = 0; i < 5; i++)
//	{
//		initValues[i] = 0;
//	}
//}
//
////set up modbus communication
//void setupComm()
//{
//	//normally you have more things to do tosetup a proper communication
//	//like port, speed, address and etc.
//	//  comm = new ModbusRTUComm(&Serial, &Serial, 9600, 2);
//	comm = new ModbusRTUComm(&Serial, 9600, 2);
//}