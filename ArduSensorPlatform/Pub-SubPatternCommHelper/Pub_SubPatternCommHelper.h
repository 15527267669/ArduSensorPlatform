/*
 Name:		Pub_SubPatternCommHelper.h
 Created:	2019/3/14 14:17:53
 Author:	jtext103
 Editor:	http://www.visualmicro.com
*/

#ifndef _Pub_SubPatternCommHelper_h
#define _Pub_SubPatternCommHelper_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include<mqttsn.h>
class Pub_SubCommHelperBase
{
public:
	Pub_SubCommHelperBase();
	~Pub_SubCommHelperBase();

	virtual int registerIntOCallback(void(*callback)())=0;
	virtual int initialize(APP_CONFIG config)=0;
	virtual void setKeepAlive(uint16_t msec)=0;
	virtual void setWillTopic(MQString* willTopic)=0;
	virtual void setWillMessage(MQString* willMsg)=0;
	virtual void setRetain(bool retain)=0;
	virtual void setClean(bool clean)=0;
	virtual void setZBPinHibernate()=0;
	virtual void startSleepMode()=0;
	virtual void addTaskInvokedByWDT(void)=0;

	virtual int registerTopic(MQString* topic)=0;
	virtual int subscribe(MQString* topic, TopicCallback callback, uint8_t qos)=0;
	virtual int subscribe(uint16_t predefinedId, TopicCallback callback, uint8_t qos = 1)=0;
	virtual int unsubscribe(MQString* topic)=0;
	virtual int	publish(MQString* topic, Payload* payload, uint8_t qos)=0;
	virtual int publish(MQString* topic, MQString* data, uint8_t qos)=0;
	virtual int publish(MQString* topic, const char* data, int dataLength, uint8_t qos)=0;
	virtual int publish(uint16_t predefinedId, const char* data, int dataLength, uint8_t qos = 1)=0;
	virtual int	disconnect(uint16_t duration)=0;

	virtual void startWdt()=0;
	virtual void stopWdt()=0;
	virtual int  run()=0;
	virtual void setUnixTime(MqttsnPublish* msg)=0;
private:

};




#endif

