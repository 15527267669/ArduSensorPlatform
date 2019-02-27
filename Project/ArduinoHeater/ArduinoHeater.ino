/*
 Name:		ArduinoHeater.ino
 Created:	2017/11/11 周六 13:51:31
 Author:	jtext103
*/


#include <MAX31855_ThermoSensor.h>
#include <PWM_PinSensor.h>
#include <SPI.h>
#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include <ModbusRTUComm.h>
#include <Timer.h>
#include <DigitalPin.h>


#ifndef USBCON

//FOR MEGA 2560
////define OUTPUT to H_bridge PIN
#define PWM1  8        //set 8 pin is PWM1 output
#define PWM2  9        //set 9 pin is PWM2 output
#define DIR1  30       //set 30 pin is dir1 output
#define DIR2  31       //set 31 pin is dir2 output

#define SSR  32       //set 32 pin is SSR output
//define SPI PIN
#define MAXDO  50        //set 50 pin is MISO
//#define MAXDO2  51        //set 51 pin is MISO2 
#define MAXCLK  52       //set 52 pin is SCLK 
//#define MAXCLK2  53       //set 53 pin is SCLK2 
#define MAXCS1  34       //set 34 pin is CS1 
#define MAXCS2  35       //set 35 pin is CS2
#else
//FOR DFROBOT leonardo
//define OUTPUT to H_bridge PIN
#define PWM1  10        //set 8 pin is PWM1 output
#define PWM2  11        //set 9 pin is PWM2 output
#define DIR1  12       //set 30 pin is dir1 output
#define DIR2  13       //set 31 pin is dir2 output

#define SSR  8       //set 32 pin is SSR output
//define SPI PIN
#define MAXDO  7        //set 50 pin is MISO
//#define MAXDO2  51        //set 51 pin is MISO2 
#define MAXCLK  6       //set 52 pin is SCLK 
//#define MAXCLK2  53       //set 53 pin is SCLK2 
#define MAXCS1  5       //set 34 pin is CS1 
#define MAXCS2  4       //set 35 pin is CS2

#endif // !USBCON


//static host declearation, but host created in setupSensor are recommanded
Host host(5);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;
Timer* sensorTimer;
Timer* commTimer;
float *initValues = NULL;


// the setup function runs once when you press reset or power the board
void setup()
{
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(500);
	commTimer = new Timer(10);
	
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
void loop()
{
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
	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS1, MAXDO);
	host.AddSensor(sensor);
	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS2, MAXDO);
	host.AddSensor(sensor);
	sensor = new PWM_PinSensor(PWM1, DIR1);
	host.AddSensor(sensor);
	sensor = new PWM_PinSensor(PWM2, DIR2);
	host.AddSensor(sensor);
	sensor = new DigitalPin(SSR,true);     
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
	comm = new ModbusRTUComm(&Serial1, 9600, 2);
}