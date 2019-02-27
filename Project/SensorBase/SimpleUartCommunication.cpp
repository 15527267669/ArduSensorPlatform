#include "SimpleUartCommunication.h"


String makeMessage(float * channelSamples, int channelNum)
{
	String message = "Values: ";
	String chNameHeader = "ch";
	for (size_t i = 0; i < channelNum; i++)
	{
		message = message + chNameHeader;
		message = message + i;
		message = message + ": ";
		message = message + channelSamples[i];
		message = message + ", ";
	}
	return message;
}


SimpleUartCommunication::SimpleUartCommunication()
{
	Serial.begin(9600);
}


SimpleUartCommunication::~SimpleUartCommunication()
{
	Serial.end();
}

int SimpleUartCommunication::HandleCommunication(Host* host)
{
	if (Serial.available() > 0) {
		// read the incoming byte:
		auto incomingByte = Serial.read();
		if (incomingByte == 'a')
		{
			Serial.println(makeMessage(host->ChannelSamples, host->TotalChannelNum));
		}
	}
}


