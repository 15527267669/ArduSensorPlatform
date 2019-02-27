#pragma once
#ifndef _RaintubeSensor_h
#define _RaintubeSensor_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
//不同板子关于“ModbusRTUComm(MySerial * port, long baud, byte slaveId);”中“port”的定义不同；
//mega2560定义的是HardwareSerial；lenoard 定义的是 Serial_
#if defined (USBCON)
#define  MySerial Serial_
#else
#define  MySerial HardwareSerial 
#endif

#include<SensorBase.h>
#include<ModbusMaster.h>


class RaintubeSensor:public SensorBase
{
public:
	RaintubeSensor(MySerial * port, long baud, byte slaveId);
	~RaintubeSensor();
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAllowed = false) override;
private:
	float rainfall;//channelNo 0
	int rainsensorAddress;//channelNo 1
	int TotalChannel = 2;//所用总的channel数量
	int result;//功能结果
	ModbusMaster node;//实例化主站节点，这里板子为主站，传感器为从站
	Stream* _Myport;//所在串口
};


#endif // !_RaintubeSensor_h

