#pragma once
#ifndef _dht11_h
#define _dht11_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2

#include<SensorBase.h>
class dht11 :public SensorBase
{
public:
	dht11(int pinNo);
	~dht11();
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAsync = false) override
	{
	}

private:
	double Fahrenheit(double celsius); //摄氏温度度转化为华氏温度
	double Kelvin(double celsius); //摄氏温度转化为开氏温度
	double dewPoint(double celsius, double humidity);// 露点（点在此温度时，空气饱和并产生露珠）
													 // 参考: http://wahiduddin.net/calc/density_algorithms.htm 
	double dewPointFast(double celsius, double humidity);// 快速计算露点，速度是5倍dewPoint()
														 // 参考: http://en.wikipedia.org/wiki/Dew_point
	//void Cheak();//检查是否超时等一些情况
	int humidity;//channelNo 1
	int temperature;//channelNo 0
	int TotalChannel=2;//所用总的channel数量
	int pinNo;//所在引脚
};



#endif // !dht11_h

