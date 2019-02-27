#pragma once


#include <Arduino.h>

#ifndef _ModbusRTUCommunication_h
#define _ModbusRTUCommunication_h

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


#include <CommunicationBase.h>
#include "dataConvertion.h"
#include <stdlib.h>

class ModbusRTUComm :public CommunicationBase
{
private:
	Stream* _Rxport;
	Stream* _Txport;
	long  _baud;
	unsigned int _t15; // inter character time out
	unsigned int _t35; // frame delay
	byte  _slaveId;
	byte _frameLength = 0;

	void processFrame(Host * host, byte * frame);
	bool checkSumOk(byte * frame);
	unsigned int _u16ReadChannelAddress;
	unsigned int _u16ReadChannelQty;
	unsigned int _u16WriteChannelAddress;
	unsigned int _u16WriteChannelQty;
	byte u8Qty;
	void runRead(Host * host, byte* frame);
	void runWrite(Host * host, byte* frame);
	void send(byte u8ModbusTransmitADUSize, byte* u8ModbusTransmitADU);
	int calcCRC(byte u8ModbusTransmitADUSize, byte* u8ModbusTransmitADU);
public:
	//ModbusRTUComm(MySerial * Rxport, HardwareSerial * Txport,long baud, byte slaveId);
	ModbusRTUComm(MySerial * port,long baud, byte slaveId);
	~ModbusRTUComm();
	// Inherited via CommunicationBase
	virtual int HandleCommunication(Host* host) override;
	
};

#endif