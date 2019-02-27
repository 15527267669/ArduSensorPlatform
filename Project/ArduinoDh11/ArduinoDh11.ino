/*
 Name:		ArduinoDh11.ino
 Created:	3/29/2018 6:43:20 PM
 Author:	jtext103
*/

#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>

#include<dht11.h>
#include <ModbusRTUComm.h>
#include <Timer.h>


//static host declearation, but host created in setupSensor are recommanded
Host host(3);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;

Timer* sensorTimer;
Timer* commTimer;



void setup() {
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(500);
	commTimer = new Timer(200);
	//start, timer may need a start function
	host.Start();
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
	sensor = new  dht11(4);
	host.AddSensor(sensor);
	sensor = new  dht11(5);
	host.AddSensor(sensor);
	sensor = new  dht11(6);
	host.AddSensor(sensor);

}

void setupComm()
{
	//normally you have more things to do tosetup a proper communication
	//like port, speed, address and etc.
	comm = new ModbusRTUComm(&Serial, 9600, 2);
}