#pragma once
#include <SensorBase.h>
#ifndef _AutoTempHummCaculator_h
#define _AutoTempHummCaculator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class AutoTempHummCaculator :public SensorBase
{
public:
	AutoTempHummCaculator(Host * host, int innertempNo, int innerhummNo, int outertempNo, int outerhummNo);
	~AutoTempHummCaculator();
	// Inherited via SensorBase
	virtual int GetChnanelNum() override;
	virtual int Update() override;
	virtual float Read(int chanelNo) override;
	virtual int Write(int channelNo, float value, bool isAllowed = false) override;
	//float sample[3];
	Host* host;
	int innertempNo;
	int innerhummNo;
	int outertempNo;
	int outerhummNo;
	float humm_inner;//内部湿度
	float temp_inner;//内部温度
	float humm_outer;//环境湿度
	float temp_outer;//环境温度

	//结果
	float humm;
	float ab_humm;//绝对湿度

};


#endif // !_AutoTempHummCaculator_h