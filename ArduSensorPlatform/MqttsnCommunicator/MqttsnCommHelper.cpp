#include"MqttsnCommHelper.h"
using namespace std;
using namespace tomyClient;
extern APP_CONFIG theAppConfig;

MqttsnCommHelper::MqttsnCommHelper()
{
}

MqttsnCommHelper::MqttsnCommHelper(APP_CONFIG appConfig)
{
	theAppConfig = appConfig;
}

MqttsnCommHelper::~MqttsnCommHelper()
{

}

int MqttsnCommHelper::registerTopic(MQString * topic)
{
	theApplication->registerTopic(topic);
	return 0;
}
int MqttsnCommHelper::subscribe(MQString* topic, TopicCallback callback, uint8_t qos)
{
	theApplication->_mqttsn.setSubscribing(true);
	theApplication->_mqttsn.subscribe(topic, callback, qos);
	theApplication->_mqttsn.setSubscribing(false);
	return 0;
}
int MqttsnCommHelper::subscribe(uint16_t predefinedId, TopicCallback callback, uint8_t qos)
{
	theApplication->subscribe(predefinedId, callback, qos);
	return 0;
}
int MqttsnCommHelper::unsubscribe(MQString* topic)
{
	theApplication->_mqttsn.unsubscribe(topic);
	return 0;
}
int MqttsnCommHelper::publish(MQString * topic, Payload * payload, uint8_t qos)
{
	theApplication->publish(topic, payload, qos);
	return 0;
}
int MqttsnCommHelper::publish(MQString * topic, MQString * data, uint8_t qos)
{
	theApplication->_mqttsn.publish(topic, data, qos);
	return 0;
}
int MqttsnCommHelper::publish(MQString * topic, const char * data, int dataLength, uint8_t qos)
{
	theApplication->publish(topic, data, dataLength, qos);
	return 0;
}
int MqttsnCommHelper::publish(uint16_t predefinedId, const char * data, int dataLength, uint8_t qos)
{
	theApplication->publish(predefinedId, data, dataLength, qos);
	return 0;
}
int MqttsnCommHelper::disconnect(uint16_t duration)
{
	theApplication->disconnect(duration);
	return 0;
}
void MqttsnCommHelper::startWdt()
{
	theApplication->startWdt();
}
void MqttsnCommHelper::stopWdt()
{
	theApplication->stopWdt();
}
int MqttsnCommHelper::run()
{
	theApplication->run();
	return 0;
}
int MqttsnCommHelper::sendRecvMsg()
{
	//call mqttsn sendRecvMsg method of theApplication 
	theApplication->_mqttsn.sendRecvMsg();
	return 0;
}
uint8_t * MqttsnCommHelper::getReceiveDataPtr()
{
	return theApplication->_mqttsn._receiveData;
}
uint8_t MqttsnCommHelper::getReceiveDatalength()
{
	return theApplication->_mqttsn._receiveDataLength;
}
void MqttsnCommHelper::setUnixTime(MqttsnPublish * msg)
{
	MqttsnCommHelper(msg);
}
int MqttsnCommHelper::registerIntOCallback(void(*callback)())
{
	theApplication->registerInt0Callback(callback);
	return 0;
}
int MqttsnCommHelper::initialize(APP_CONFIG config)
{
	theApplication->initialize(config);
	return 0;
}
void MqttsnCommHelper::setKeepAlive(uint16_t msec)
{
	theApplication->setKeepAlive(msec);
}
void MqttsnCommHelper::setWillTopic(MQString * willTopic)
{
	theApplication->setWillTopic(willTopic);
}
void MqttsnCommHelper::setWillMessage(MQString * willMsg)
{
	theApplication->setWillMessage(willMsg);
}
void MqttsnCommHelper::setRetain(bool retain)
{
	theApplication->setRetain(retain);
}
void MqttsnCommHelper::setClean(bool clean)
{
	theApplication->setClean(clean);
}
void MqttsnCommHelper::setZBPinHibernate()
{
	theApplication->setZBPinHibernate();
}
void MqttsnCommHelper::startSleepMode()
{
	theApplication->startSleepMode();
}
void MqttsnCommHelper::addTaskInvokedByWDT(void)
{
	theApplication->addTask();
}
