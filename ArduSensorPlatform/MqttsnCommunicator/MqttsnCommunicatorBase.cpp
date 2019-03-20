/*
 Name:		MqttsnCommunicator.cpp
 Created:	2019/2/28 8:43:09
 Author:	jtext103
 Editor:	http://www.visualmicro.com
*/

#include "MqttsnCommunicatorBase.h"
using namespace std;
using namespace tomyClient;
extern APP_CONFIG theAppConfig;

MqttsnComm::MqttsnComm()
{
}

MqttsnComm::MqttsnComm(APP_CONFIG appConfig)
{
	theAppConfig = appConfig;
}

MqttsnComm::~MqttsnComm()
{
}


int MqttsnComm::registerTopic(MQString * topic)
{
	theApplication->registerTopic(topic);
	return 0;
}
int MqttsnComm::subscribe(MQString* topic, TopicCallback callback, uint8_t qos)
{
	theApplication->_mqttsn.setSubscribing(true);
	theApplication->_mqttsn.subscribe(topic, callback,  qos);
	theApplication->_mqttsn.setSubscribing(false);
	return 0;
}
int MqttsnComm::subscribe(uint16_t predefinedId, TopicCallback callback, uint8_t qos)
{
	theApplication->subscribe( predefinedId,  callback,  qos);
	return 0;
}
int MqttsnComm::unsubscribe(MQString* topic)
{
	theApplication->_mqttsn.unsubscribe(topic);
	return 0;
}
int MqttsnComm::publish(MQString * topic, Payload * payload, uint8_t qos)
{
	theApplication->publish(topic, payload, qos);
	return 0;
}
int MqttsnComm::publish(MQString * topic, MQString * data, uint8_t qos)
{
	theApplication->_mqttsn.publish(topic, data, qos);
	return 0;
}
int MqttsnComm::publish(MQString * topic, const char * data, int dataLength, uint8_t qos)
{
	theApplication->publish(topic, data, dataLength, qos);
	return 0;
}
int MqttsnComm::publish(uint16_t predefinedId, const char * data, int dataLength, uint8_t qos)
{
	theApplication->publish( predefinedId,   data,  dataLength,  qos);
	return 0;
}
int MqttsnComm::disconnect(uint16_t duration)
{
	theApplication->disconnect(duration);
	return 0;
}
void MqttsnComm::startWdt()
{
	theApplication->startWdt();
}
void MqttsnComm::stopWdt()
{
	theApplication->stopWdt();
}
int MqttsnComm::run()
{
	theApplication->run();
	return 0;
}
void MqttsnComm::setUnixTime(MqttsnPublish * msg)
{
	setUnixTime(msg);
}
int MqttsnComm::registerIntOCallback(void(*callback)())
{
	theApplication->registerInt0Callback(callback);
	return 0;
}
int MqttsnComm::initialize(APP_CONFIG config)
{
	theApplication->initialize(config);
	return 0;
}
void MqttsnComm::setKeepAlive(uint16_t msec)
{
	theApplication->setKeepAlive(msec);
}
void MqttsnComm::setWillTopic(MQString * willTopic)
{
	theApplication->setWillTopic(willTopic);
}
void MqttsnComm::setWillMessage(MQString * willMsg)
{
	theApplication->setWillMessage(willMsg);
}
void MqttsnComm::setRetain(bool retain)
{
	theApplication->setRetain(retain);
}
void MqttsnComm::setClean(bool clean)
{
	theApplication->setClean(clean);
}
void MqttsnComm::setZBPinHibernate()
{
	theApplication->setZBPinHibernate();
}
void MqttsnComm::startSleepMode()
{
	theApplication->startSleepMode();
}
void MqttsnComm::addTaskInvokedByWDT(void)
{
	theApplication->addTask();
}

///*-----------------------------------------------
//				 Task region
//------------------------------------------------*/
//
///*---------------  List of task invoked by Timer ------------*/
//void MqttsnComm::update()
//{//Periodically publish the latest values of each channel in the host
//	//TASK_LIST = {//{ MQString* topic, executing duration in second},
//	//	{publish_ChannelValue, 40},
//	//	END_OF_TASK_LIST
//	//};
//}
//
//void MqttsnComm::creatTasklist()
//{
//
//}


