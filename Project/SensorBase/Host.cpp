#include "Host.h"



Host::Host(int totalSensorNum)
{
	//create sensor array
	sensors = new SensorBase*[totalSensorNum];
	this->totalSensorNum = totalSensorNum;
}

Host::~Host()
{
	//is your responsibiliti to delete all the garbages
	for (size_t i = 0; i < totalSensorNum; i++)
	{
		delete sensors[i];
	}
	delete[] sensors;
	delete[] ChannelSamples;
}

int Host::AddSensor(SensorBase * sensor)
{
	if (sensorAddedNum < totalSensorNum)
	{	// add only the sensor has not exsit the totoal sensor num
		sensors[sensorAddedNum] = sensor;
		sensorAddedNum++;
		//add channel by the channels this sensor has
		TotalChannelNum += sensor->GetChnanelNum();
		return sensorAddedNum;
	}
	//error
	return -1;

}

int Host::Start()
{
	if (sensorAddedNum != totalSensorNum)
	{
		//sensor not added completely
		return -1;
	}
	ChannelSamples = new float[TotalChannelNum];
	//from now on you can do update
	isHostValid = true;
	
}

//start with init channel values
int Host::Start(float * initChanelValues)
{
	Start();
	if (initChanelValues == NULL)
	{
		return;
	}
	for (int i = 0; i < TotalChannelNum; i++)
	{
		ChannelSamples[i] = initChanelValues[i];
	}
}

int Host::Update()
{
	int onChanel = 0;
	//for each sensor
	for (size_t i = 0; i < totalSensorNum; i++)
	{
		//update the sensor
		sensors[i]->Update();
		//read out the value
		auto channelNumOfThisSensor = sensors[i]->GetChnanelNum();
		for (size_t ch = 0; ch < channelNumOfThisSensor; ch++)
		{
			//write host channel samples into sensor channel
			sensors[i]->Write(ch, ChannelSamples[onChanel]);
			//read values into channel samples
			ChannelSamples[onChanel] = sensors[i]->Read(ch);
			onChanel++;
		}
		/*delay(200);*/
	}
}

bool Host::GetIsHostValid()
{
	return isHostValid;
}

