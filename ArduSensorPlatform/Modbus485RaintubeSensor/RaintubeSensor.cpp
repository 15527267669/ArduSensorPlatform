#include "RaintubeSensor.h"

RaintubeSensor::RaintubeSensor(MySerial * port, long baud, byte slaveId)
{
	this->_Myport = port;
	(*port).begin(baud);
	rainsensorAddress = slaveId;//initialize the rainsensorAddress
	node.begin(slaveId, *_Myport);
}

RaintubeSensor::~RaintubeSensor()
{
}

int RaintubeSensor::GetChnanelNum()
{
	return TotalChannel;
}

int RaintubeSensor::Update()
{
	result=node.readHoldingRegisters(0x002a, 0x0001);
	
		uint16_t _responseBuffer;
		_responseBuffer = node.getResponseBuffer(0);
		rainfall = _responseBuffer*0.1;
	rainsensorAddress = node.readHoldingRegisters(0x4000, 0x0001);
		_responseBuffer = node.getResponseBuffer(0);
		rainsensorAddress= _responseBuffer;
	return 0;
}

float RaintubeSensor::Read(int chanelNo)
{

	switch (chanelNo)
	{
	case 0:return rainfall;
		break;
	case 1:return rainsensorAddress;
		break;
	}
}

int RaintubeSensor::Write(int channelNo, float value, bool isAllowed)
{
	switch (channelNo)
	{
	case 0:return 0;
		break;
	case 1:
		if(isAllowed==true)
		node.writeSingleRegister(0x4000, value);//ÐÞ¸ÄsensorµØÖ·
		return 0;
		break;
	}
}
