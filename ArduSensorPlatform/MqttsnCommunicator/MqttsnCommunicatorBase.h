/*
 Name:		MqttsnCommunicator.h
 Created:	2019/2/28 8:43:09
 Author:	jtext103
 Editor:	http://www.visualmicro.com
*/

#ifndef _MqttsnCommHelper_h
#define _MqttsnCommHelper_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define ARDU_SENSOR_PLATFORM

#include<CommunicationBase.h>
#include <MQTTSN_Application.h>
#include <mqttsnClientAppFw4Arduino.h>
#include<SPI.h>
#include<Ethernet.h>
#include <EthernetUdp.h>

extern void interruptCallback(void);

class MqttsnComm:public CommunicationBase
{
public:
	MqttsnComm();
	MqttsnComm(APP_CONFIG appConfig);//配置网络配置和Mqttsn配置；其中mqttsn配置为：ClientId、KeepAlive、KeepAlive、EndDevice、WillTopic、WillMessage的相关信息
	~MqttsnComm();

	int registerIntOCallback(void(*callback)());
	int initialize(APP_CONFIG config);
	void setKeepAlive(uint16_t msec);
	void setWillTopic(MQString* willTopic);
	void setWillMessage(MQString* willMsg);
	void setRetain(bool retain);
	void setClean(bool clean);
	void setZBPinHibernate();
	void startSleepMode();
	void addTaskInvokedByWDT(void);

	int registerTopic(MQString* topic);
	int subscribe(MQString* topic, TopicCallback callback, uint8_t qos);
	int subscribe(uint16_t predefinedId, TopicCallback callback, uint8_t qos = 1);
	int unsubscribe(MQString* topic);
	int	publish(MQString* topic, Payload* payload, uint8_t qos);
	int publish(MQString* topic, MQString* data, uint8_t qos);
	int publish(MQString* topic, const char* data, int dataLength, uint8_t qos);
	int publish(uint16_t predefinedId, const char* data, int dataLength, uint8_t qos = 1);
	int	disconnect(uint16_t duration);
	
	void startWdt();
	void stopWdt();
	int  run();
	void setUnixTime(MqttsnPublish* msg);
	
	

private:

	//void update();
	//void creatTasklist();
	

};



#endif

