#include "FanswitchPin.h"

FanswitchPin::FanswitchPin(int pinNo, Host*host, int hummchannelNumber)
{
	this->pinNo = pinNo;
	this->host = host;
	this->hummchannelNumber = hummchannelNumber;
	humm = 0;
	setPIN();
	//init
	delay(200);
	Write(0, 0);
}

FanswitchPin::~FanswitchPin()
{
}

int FanswitchPin::GetChnanelNum()
{
	return 1;
}

int FanswitchPin::Update()
{
	humm = host->ChannelSamples[hummchannelNumber];
	Handle(humm);
	return 0;
}

float FanswitchPin::Read(int chanelNo)
{
	return sample[0];
}

int FanswitchPin::Write(int channelNo, float value, bool isAllowed)
{
	if (isAllowed == true)
	{
		sample[0] = value;
		//this->Update();
		return 0;
	}
	
}

void FanswitchPin::Handle(float humm)
{
	if (humm>60)
	{
		sample[0]=1;
	}
	else
	{
		sample[0] = 0;
	}
	/*if (sample[0]==1)
	{
		digitalWrite(pinNo, HIGH);
	}
	else
	{
		digitalWrite(pinNo, LOW);
	}*/
}

void FanswitchPin::setPIN()
{
	pinMode(pinNo, OUTPUT);
}
