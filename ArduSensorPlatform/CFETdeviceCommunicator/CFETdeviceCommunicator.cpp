/*
 Name:		CFETdeviceCommunicator.cpp
 Created:	2019/3/14 11:24:56
 Author:	jtext103
 Editor:	http://www.visualmicro.com
*/

#include "CFETdeviceCommunicator.h"

CFETdeviceCommunicator::CFETdeviceCommunicator()
{
}

CFETdeviceCommunicator::CFETdeviceCommunicator(CfetDeviceCommConfig * config)
{
	_slaveName = new MQString(config->slaveName);
	_channelDescrip = new MQString(config->channelDescrip);
	_discTopic = new MQString(config->discTopic);
	_regisTopic = new MQString(config->regisTopic);
	_regisConfirmTopic = new MQString(config->regisConfirmTopic);
	_publishTopic= new MQString(config->publishTopic);
	_sensorPingTopic= new MQString(config->sensorPingTopic);
	_cfetPingTopic= new MQString(config->cfetPingTopic);
	_cfetReadTopic= new MQString(config->cfetReadTopic);
	_cfetWriteTopic= new MQString(config->cfetWriteTopic);
	_cfetStatus = true;
	_ChannelUpdated = -1;
}

CFETdeviceCommunicator::~CFETdeviceCommunicator()
{
	//is your responsibility to delete garbage
	delete _pubsubCommHelper;
	delete _slaveName;
	delete _channelDescrip;
	delete _discTopic;
	delete _regisTopic;
	delete _regisConfirmTopic;
	delete _publishTopic;
	delete _sensorPingTopic;
	delete _cfetPingTopic;
	delete _cfetReadTopic;
	delete _cfetWriteTopic;
}

int CFETdeviceCommunicator::HandleCommunication(Host * host)
{
	//start

	return 0;
}

void CFETdeviceCommunicator::setUpdateChannel(uint8_t updateChannel)
{
	_ChannelUpdated = updateChannel;
}

void CFETdeviceCommunicator::LoadCommhelper(Pub_SubCommHelperBase * pubsubCommhelper)
{
	_pubsubCommHelper = pubsubCommhelper;
}

void CFETdeviceCommunicator::start()
{
	//创建发现Topic，传感器平台订阅发现TOPIC,设置订阅发现TOPIC的回调函数
	//_discTopic = new MQString("rainTube_1/registerTopic");这一步留到应用中在创建CfetDeviceCommConfig时做
	_pubsubCommHelper->subscribe(_discTopic,this->discCallback , QOS1);
}

int CFETdeviceCommunicator::discCallback(MqttsnPublish* registerMsg)
{
	//生成注册topic报文
#pragma region regisTopic generation
	Payload regisPayload = Payload(300);
	//注册报文头
	regisPayload.set_str("REG");
	regisPayload.set_str(";");
	//确认注册topic
	regisPayload.set_str(_regisConfirmTopic->getStr);
	regisPayload.set_str(";");
	//报文类型
	regisPayload.set_str("S");
	regisPayload.set_str(";");
	//传感器平台名字
	regisPayload.set_str(_slaveName->getStr);
	regisPayload.set_str(";");
	//发布topic
	regisPayload.set_str(_publishTopic->getStr);
	regisPayload.set_str(";");
	//CFETpingtopic
	regisPayload.set_str(_cfetPingTopic->getStr);
	regisPayload.set_str(";");
	//传感器pingtopic
	regisPayload.set_str(_sensorPingTopic->getStr);
	regisPayload.set_str(";");
	//cfet读topic
	regisPayload.set_str(_cfetReadTopic->getStr);
	regisPayload.set_str(";");
	//cfet写topic
	regisPayload.set_str(_cfetWriteTopic->getStr);
	regisPayload.set_str(";");
	////心跳超时上限，单位s
	//regisPayload.set_str("3600");
	//regisPayload.set_str(";");
	//通道描述
	regisPayload.set_str(_channelDescrip->getStr);
	//over
	regisPayload.set_str("$");
#pragma endregion
	//发布注册Topic
	_pubsubCommHelper->publish(_regisTopic, &regisPayload, QOS1);
	//订阅注册确认topic
	_pubsubCommHelper->subscribe(_regisConfirmTopic,this->regisConfirmCallback, QOS1);
	//订阅CFETpingTopic
	_pubsubCommHelper->subscribe(_cfetPingTopic, this->cfetpingCallback, QOS1);
	//订阅CFETreadTopic
	_pubsubCommHelper->subscribe(_cfetReadTopic, this->cfetReadCallback, QOS1);
	//订阅CFETwriteTopic
	_pubsubCommHelper->subscribe(_cfetWriteTopic, this->cfetWriteCallback, QOS1);
	return 0;
}

int CFETdeviceCommunicator::regisConfirmCallback(MqttsnPublish * registerMsg)
{
	//生成传感器心跳topic
#pragma region sensorPingTopic generation
	Payload sensorPingPayload = Payload(10);
	//报文头
	sensorPingPayload.set_str("REG");
	sensorPingPayload.set_str(";");
	//心跳超时秒数
	sensorPingPayload.set_str("1800");
	sensorPingPayload.set_str("$");
#pragma endregion
	//发布传感器心跳topic
	_pubsubCommHelper->publish(_sensorPingTopic, &sensorPingPayload, QOS1);
	//取消订阅发现topic和注册确认Topic
	_pubsubCommHelper->unsubscribe(_discTopic);
	_pubsubCommHelper->unsubscribe(_regisConfirmTopic);
	return 0;
}

int CFETdeviceCommunicator::cfetpingCallback(MqttsnPublish * registerMsg)
{
	//初始化计时器
	_sensorPingTimer = millis();
	return 0;
}

int CFETdeviceCommunicator::cfetReadCallback(MqttsnPublish * registerMsg)
{
	
	return 0;
}

int CFETdeviceCommunicator::cfetWriteCallback(MqttsnPublish * registerMsg)
{
	return 0;
}

void CFETdeviceCommunicator::checkOnline()
{
	auto now = millis();
	if ((now - _sensorPingTimer) > _maxPingInterval) //consider overflow here
	{
		_cfetStatus = false;
	}
}

void CFETdeviceCommunicator::checkChUpdated(Host * host)
{
	if (_ChannelUpdated!=-1)
	{
		//生成发布传感器通道值报文
#pragma region sensorValueUpdatedPublishTopic
		Payload sensorPingPayload = Payload(35);
		//报文头
		sensorPingPayload.set_str("PUB");
		sensorPingPayload.set_str(";");
		//update标志
		sensorPingPayload.set_str("UPD");
		sensorPingPayload.set_str(";");
		//报文主体
		auto channelValue = host->ChannelSamples[_ChannelUpdated];//get the channelvalue update from host
		char channelUadate[5];
		itoa(_ChannelUpdated, channelUadate, 10);
		sensorPingPayload.set_str(channelUadate);//填更新的通道号
		sensorPingPayload.set_str(";");
		char channelvalue[10];
		dtostrf(channelValue, 1, 2, channelvalue);
		sensorPingPayload.set_str(channelvalue);//填更新的通道值
		sensorPingPayload.set_str("$");
#pragma endregion

		_pubsubCommHelper->publish(_publishTopic,&sensorPingPayload,QOS1);
	}
}

