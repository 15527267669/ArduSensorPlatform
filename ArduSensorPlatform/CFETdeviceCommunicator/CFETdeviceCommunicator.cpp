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
	//��������Topic��������ƽ̨���ķ���TOPIC,���ö��ķ���TOPIC�Ļص�����
	//_discTopic = new MQString("rainTube_1/registerTopic");��һ������Ӧ�����ڴ���CfetDeviceCommConfigʱ��
	_pubsubCommHelper->subscribe(_discTopic,this->discCallback , QOS1);
}

int CFETdeviceCommunicator::discCallback(MqttsnPublish* registerMsg)
{
	//����ע��topic����
#pragma region regisTopic generation
	Payload regisPayload = Payload(300);
	//ע�ᱨ��ͷ
	regisPayload.set_str("REG");
	regisPayload.set_str(";");
	//ȷ��ע��topic
	regisPayload.set_str(_regisConfirmTopic->getStr);
	regisPayload.set_str(";");
	//��������
	regisPayload.set_str("S");
	regisPayload.set_str(";");
	//������ƽ̨����
	regisPayload.set_str(_slaveName->getStr);
	regisPayload.set_str(";");
	//����topic
	regisPayload.set_str(_publishTopic->getStr);
	regisPayload.set_str(";");
	//CFETpingtopic
	regisPayload.set_str(_cfetPingTopic->getStr);
	regisPayload.set_str(";");
	//������pingtopic
	regisPayload.set_str(_sensorPingTopic->getStr);
	regisPayload.set_str(";");
	//cfet��topic
	regisPayload.set_str(_cfetReadTopic->getStr);
	regisPayload.set_str(";");
	//cfetдtopic
	regisPayload.set_str(_cfetWriteTopic->getStr);
	regisPayload.set_str(";");
	////������ʱ���ޣ���λs
	//regisPayload.set_str("3600");
	//regisPayload.set_str(";");
	//ͨ������
	regisPayload.set_str(_channelDescrip->getStr);
	//over
	regisPayload.set_str("$");
#pragma endregion
	//����ע��Topic
	_pubsubCommHelper->publish(_regisTopic, &regisPayload, QOS1);
	//����ע��ȷ��topic
	_pubsubCommHelper->subscribe(_regisConfirmTopic,this->regisConfirmCallback, QOS1);
	//����CFETpingTopic
	_pubsubCommHelper->subscribe(_cfetPingTopic, this->cfetpingCallback, QOS1);
	//����CFETreadTopic
	_pubsubCommHelper->subscribe(_cfetReadTopic, this->cfetReadCallback, QOS1);
	//����CFETwriteTopic
	_pubsubCommHelper->subscribe(_cfetWriteTopic, this->cfetWriteCallback, QOS1);
	return 0;
}

int CFETdeviceCommunicator::regisConfirmCallback(MqttsnPublish * registerMsg)
{
	//���ɴ���������topic
#pragma region sensorPingTopic generation
	Payload sensorPingPayload = Payload(10);
	//����ͷ
	sensorPingPayload.set_str("REG");
	sensorPingPayload.set_str(";");
	//������ʱ����
	sensorPingPayload.set_str("1800");
	sensorPingPayload.set_str("$");
#pragma endregion
	//��������������topic
	_pubsubCommHelper->publish(_sensorPingTopic, &sensorPingPayload, QOS1);
	//ȡ�����ķ���topic��ע��ȷ��Topic
	_pubsubCommHelper->unsubscribe(_discTopic);
	_pubsubCommHelper->unsubscribe(_regisConfirmTopic);
	return 0;
}

int CFETdeviceCommunicator::cfetpingCallback(MqttsnPublish * registerMsg)
{
	//��ʼ����ʱ��
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
		//���ɷ���������ͨ��ֵ����
#pragma region sensorValueUpdatedPublishTopic
		Payload sensorPingPayload = Payload(35);
		//����ͷ
		sensorPingPayload.set_str("PUB");
		sensorPingPayload.set_str(";");
		//update��־
		sensorPingPayload.set_str("UPD");
		sensorPingPayload.set_str(";");
		//��������
		auto channelValue = host->ChannelSamples[_ChannelUpdated];//get the channelvalue update from host
		char channelUadate[5];
		itoa(_ChannelUpdated, channelUadate, 10);
		sensorPingPayload.set_str(channelUadate);//����µ�ͨ����
		sensorPingPayload.set_str(";");
		char channelvalue[10];
		dtostrf(channelValue, 1, 2, channelvalue);
		sensorPingPayload.set_str(channelvalue);//����µ�ͨ��ֵ
		sensorPingPayload.set_str("$");
#pragma endregion

		_pubsubCommHelper->publish(_publishTopic,&sensorPingPayload,QOS1);
	}
}

