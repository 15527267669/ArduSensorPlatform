#include "DigitalPin.h"




DigitalPin::DigitalPin(int pinNo, bool isOutput)
{
	//every sensor has different constructor
	//
	this->pinNo = pinNo;
	this->isOutput = isOutput;
	auto myPinMode = INPUT;
	if (isOutput)
	{
		myPinMode = OUTPUT;
	}
	pinMode(this->pinNo, myPinMode);
	//init
	delay(200);
	Write(0,0);

}

DigitalPin::~DigitalPin()
{
}

int DigitalPin::GetChnanelNum()
{
	return 1;
}

int DigitalPin::Update()
{
	//in this exsample we have only one channel
	//here we fetch value from our sensor hardware and give it to the sample array
	if (isOutput)
	{
		//no matter if we are using async update or note update always update the out put
		pinMode(this->pinNo, OUTPUT);
		digitalWrite(this->pinNo,sample[0]);
	}
	else
	{
		//wee have only one channel so channelNo is ignored
		pinMode(this->pinNo,INPUT);
		sample[0] = digitalRead(this->pinNo);

	}
	return 0;
}

float DigitalPin::Read(int chanelNum)
{
	//this is simple just return the right sample
	//wee have only one channel so channelNo is ignored
	return sample[0];
}

int DigitalPin::Write(int channelNo, float value, bool isAsync = false)
{
	if (!isOutput)
	{
		return 1;	//1 means write is not supported
	}
	//wee have only one channel so channelNo is ignored
	//always set the sample even if the action is sync and you update the value here
	sample[0] = value;
	if (!isAsync)
	{
		//if it is sync action, update imediatly after you set the sample
		//you can explicitly update the channel you update instead of just call update
		//like£º digitalWrite(mapChannelToPin(pinNo),value);
		this->Update();
	}
	return 0;
}
