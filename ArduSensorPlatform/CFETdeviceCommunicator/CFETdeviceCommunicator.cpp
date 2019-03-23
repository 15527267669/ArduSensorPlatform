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
	LoadCommhelper(config->Pub_SubCommHelper);
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
	_host = host;
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
	Payload regisPayload = Payload(69);//frame max = 70 byte
	//ע�ᱨ��ͷ
	regisPayload.set_str("REG");
	regisPayload.set_str(";");
	//ȷ��ע��topic
	regisPayload.set_str(_regisConfirmTopic->getStr);
	regisPayload.set_str(";");
	////��������
	//regisPayload.set_str("S");
	//regisPayload.set_str(";");
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
	Payload sensorPingPayload = Payload(20);
	//����ͷ
	sensorPingPayload.set_str("PING");
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
	//����_pubsubCommHelper���շ���Ϣ�����������յ��ı��Ĵ�Xbee��������ȡMQTT-S���ģ��ٴ�MQTT-S������ȡCFET device Э�鱨��
	runRead(_host);

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

//void CFETdeviceCommunicator::analyzeReceiveData()
//{
//	//����_pubsubCommHelper���շ���Ϣ�����������յ��ı��Ĵ�Xbee��������ȡMQTT-S���ģ��ٴ�MQTT-S������ȡCFET device Э�鱨��
//	_pubsubCommHelper->sendRecvMsg();
//	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//��ʱreceiveDataPtr�����CFET device Э�鱨�ĵĳ�ʼ��ַ
//	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//��ʱreceiveDataPtr�����CFET device Э�鱨�ĵĳ���
//	for (unsigned int i = 0; i < _receiveDataLength; i++)
//	{
//		char field[30];
//		if ((_receiveDataPtr[i] == ';') || (_receiveDataPtr[i] == '$'))
//		{
//			if (strcmp(field, "GET")==0)
//			{
//				runRead();
//			} 
//			else if (strcmp(field, "SET") == 0)
//			{
//				runWrite();
//			}
//			break;
//		}
//		else
//		{
//			field[i] = _receiveDataPtr[i];
//		}
//	}
//
//}

void CFETdeviceCommunicator::runRead(Host* host)
{
	//����_pubsubCommHelper���շ���Ϣ�����������յ��ı��Ĵ�Xbee��������ȡMQTT-S���ģ��ٴ�MQTT-S������ȡCFET device Э�鱨��
	_pubsubCommHelper->sendRecvMsg();
	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//��ʱreceiveDataPtr�����CFET device Э�鱨�ĵĳ�ʼ��ַ
	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//��ʱreceiveDatalength�����CFET device Э�鱨�ĵĳ���
	auto i = 0;
	unsigned int count = 0;
	String fieldString = "";
	unsigned int channelNumber[50];
	while (i<_receiveDataLength)
	{
		if ((_receiveDataPtr[i] == ';') || (_receiveDataPtr[i] == '$'))
		{
			//��һ���ǡ�GET��������
			if (count == 0)
			{
				if (fieldString=="GET")
				{
					//�ֶ��ַ����ع��ʼֵ
					fieldString = "";
					//�ֶμ���+1
					count++;
				}
				else
				{
					break;
				}	
			}
			//���˵�һ��������ȫ��ͨ����
			else
			{
				//ȡ�����ֶ�ת��int�����int����Ҫ�õ�ͨ����
				channelNumber[count-1] = fieldString.toInt();
				//�ֶ��ַ����ع��ʼֵ
				fieldString = "";
				//�ֶμ���+1
				count++;
			}
			i++;
		}
		else
		{
			//������Ƿָ������������߽�������$�����������ַ��Ž��ֶ��ַ�����
			fieldString += _receiveDataPtr[i];
			i++;
		}
	}
	//��host��ÿ��ͨ����ֵ���publishTopic payload��ȥ
	//���ɷ���������ͨ��ֵ����
#pragma region sensorValueReadPublishTopic
	Payload sensorPingPayload = Payload(69);
	//����ͷ
	sensorPingPayload.set_str("PUB");
	sensorPingPayload.set_str(";");
	//read��־
	sensorPingPayload.set_str("GET");
	sensorPingPayload.set_str(";");
	//��������
	float channelValue;
	for (unsigned int i=0; i < (count-2); i++)
	{
		channelValue = host->ChannelSamples[channelNumber[i]];//get the channelvalue update from host
		char channelRead[5];
		itoa(channelNumber[i], channelRead, 10);
		sensorPingPayload.set_str(channelRead);//�����ͨ����
		sensorPingPayload.set_str(";");
		char channelval[10];
		dtostrf(channelValue, 1, 2, channelval);
		sensorPingPayload.set_str(channelval);//�����ͨ��ֵ
		sensorPingPayload.set_str(";");
	}
	channelValue = host->ChannelSamples[channelNumber[count-2]];//get the channelvalue update from host
	char channelRead[5];
	itoa(channelNumber[count - 2], channelRead, 10);
	sensorPingPayload.set_str(channelRead);//�����ͨ����
	sensorPingPayload.set_str(";");
	char channelval[10];
	dtostrf(channelValue, 1, 2, channelval);
	sensorPingPayload.set_str(channelval);//�����ͨ��ֵ
	sensorPingPayload.set_str("$");
#pragma endregion
	//����
	_pubsubCommHelper->publish(_publishTopic, &sensorPingPayload, QOS1);

}

void CFETdeviceCommunicator::runWrite(Host* host)
{
	//����_pubsubCommHelper���շ���Ϣ�����������յ��ı��Ĵ�Xbee��������ȡMQTT-S���ģ��ٴ�MQTT-S������ȡCFET device Э�鱨��
	_pubsubCommHelper->sendRecvMsg();
	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//��ʱreceiveDataPtr�����CFET device Э�鱨�ĵĳ�ʼ��ַ
	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//��ʱreceiveDatalength�����CFET device Э�鱨�ĵĳ���
	auto i = 0;
	unsigned int count = 0;
	String fieldString = "";
	unsigned int channelNumber[50];//��Ҫд��ͨ����
	float channelValue[50];//��Ҫд��ͨ��ֵ���������ͨ����һһ��Ӧ
	//���յ�����Ϣ�ж���Ҫд��ͨ���ź�ͨ��ֵ
	while (i<_receiveDataLength)
	{
		if ((_receiveDataPtr[i] == ';') || (_receiveDataPtr[i] == '$'))
		{
			//��һ���ǡ�SET��������
			if (count == 0)
			{
				if (fieldString == "SET")
				{
					//�ֶ��ַ����ع��ʼֵ
					fieldString = "";
					//�ֶμ���+1
				}
				else
				{
					break;
				}
			}
			//���˵�һ��,�ӵڶ��ο�ʼ��ż���Σ���ӦcountΪ��������ͨ���ţ������Σ���ӦcountΪż������ͨ��ֵ
			else
			{
				if ((count&1)==0)//ż��count
				{
					//ȡ�����ֶ�ת��float�����float����Ҫд��ͨ��ֵ
					channelValue[(count>>1) - 1] = fieldString.toFloat();
				}
				else
				{
					//ȡ�����ֶ�ת��int�����int����Ҫ�õ�ͨ����
					channelNumber[count>>1] = fieldString.toInt();
				}
				//�ֶ��ַ����ع��ʼֵ
				fieldString = "";	
			}
			//�ֶμ���+1
			count++;
			i++;
		}
		else
		{
			//������Ƿָ������������߽�������$�����������ַ��Ž��ֶ��ַ�����
			fieldString += _receiveDataPtr[i];
			i++;
		}
	}
	//����host����Ӧ��ͨ��
	for (unsigned int i = 0; i < (count>>1); i++)//count����2ȡ��ΪҪд��ͨ������
	{
		host->ChannelSamples[channelNumber[i]] = channelValue[i];
	}
	//���ɻظ�дָ���publish����
#pragma region WriteResponsePub generation
	Payload WriteResponsePub = Payload(69);
	//����ͷ
	WriteResponsePub.set_str("PUB");
	WriteResponsePub.set_str(";");
	//дָ��ظ���־
	WriteResponsePub.set_str("SET");
	WriteResponsePub.set_str(";");
	//��������
	WriteResponsePub.set_str("SUCCESS");
	WriteResponsePub.set_str("$");
#pragma endregion
	//����
	_pubsubCommHelper->publish(_publishTopic, &WriteResponsePub, QOS1);

}

