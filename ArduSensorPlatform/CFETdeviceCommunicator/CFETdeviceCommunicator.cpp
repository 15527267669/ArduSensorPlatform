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
	//创建发现Topic，传感器平台订阅发现TOPIC,设置订阅发现TOPIC的回调函数
	//_discTopic = new MQString("rainTube_1/registerTopic");这一步留到应用中在创建CfetDeviceCommConfig时做
	_pubsubCommHelper->subscribe(_discTopic,this->discCallback , QOS1);
}

int CFETdeviceCommunicator::discCallback(MqttsnPublish* registerMsg)
{
	//生成注册topic报文
#pragma region regisTopic generation
	Payload regisPayload = Payload(69);//frame max = 70 byte
	//注册报文头
	regisPayload.set_str("REG");
	regisPayload.set_str(";");
	//确认注册topic
	regisPayload.set_str(_regisConfirmTopic->getStr);
	regisPayload.set_str(";");
	////报文类型
	//regisPayload.set_str("S");
	//regisPayload.set_str(";");
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
	Payload sensorPingPayload = Payload(20);
	//报文头
	sensorPingPayload.set_str("PING");
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
	//调用_pubsubCommHelper的收发消息处理函数，将收到的报文从Xbee报文中提取MQTT-S报文，再从MQTT-S报文提取CFET device 协议报文
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

//void CFETdeviceCommunicator::analyzeReceiveData()
//{
//	//调用_pubsubCommHelper的收发消息处理函数，将收到的报文从Xbee报文中提取MQTT-S报文，再从MQTT-S报文提取CFET device 协议报文
//	_pubsubCommHelper->sendRecvMsg();
//	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//此时receiveDataPtr存的是CFET device 协议报文的初始地址
//	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//此时receiveDataPtr存的是CFET device 协议报文的长度
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
	//调用_pubsubCommHelper的收发消息处理函数，将收到的报文从Xbee报文中提取MQTT-S报文，再从MQTT-S报文提取CFET device 协议报文
	_pubsubCommHelper->sendRecvMsg();
	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//此时receiveDataPtr存的是CFET device 协议报文的初始地址
	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//此时receiveDatalength存的是CFET device 协议报文的长度
	auto i = 0;
	unsigned int count = 0;
	String fieldString = "";
	unsigned int channelNumber[50];
	while (i<_receiveDataLength)
	{
		if ((_receiveDataPtr[i] == ';') || (_receiveDataPtr[i] == '$'))
		{
			//第一段是“GET”，忽略
			if (count == 0)
			{
				if (fieldString=="GET")
				{
					//字段字符串回归初始值
					fieldString = "";
					//字段计数+1
					count++;
				}
				else
				{
					break;
				}	
			}
			//除了第一段其他段全是通道号
			else
			{
				//取出该字段转成int，这个int就是要用的通道号
				channelNumber[count-1] = fieldString.toInt();
				//字段字符串回归初始值
				fieldString = "";
				//字段计数+1
				count++;
			}
			i++;
		}
		else
		{
			//如果不是分隔符“；”或者结束符“$”，把数据字符放进字段字符串里
			fieldString += _receiveDataPtr[i];
			i++;
		}
	}
	//从host读每个通道的值，填到publishTopic payload中去
	//生成发布传感器通道值报文
#pragma region sensorValueReadPublishTopic
	Payload sensorPingPayload = Payload(69);
	//报文头
	sensorPingPayload.set_str("PUB");
	sensorPingPayload.set_str(";");
	//read标志
	sensorPingPayload.set_str("GET");
	sensorPingPayload.set_str(";");
	//报文主体
	float channelValue;
	for (unsigned int i=0; i < (count-2); i++)
	{
		channelValue = host->ChannelSamples[channelNumber[i]];//get the channelvalue update from host
		char channelRead[5];
		itoa(channelNumber[i], channelRead, 10);
		sensorPingPayload.set_str(channelRead);//填读的通道号
		sensorPingPayload.set_str(";");
		char channelval[10];
		dtostrf(channelValue, 1, 2, channelval);
		sensorPingPayload.set_str(channelval);//填读的通道值
		sensorPingPayload.set_str(";");
	}
	channelValue = host->ChannelSamples[channelNumber[count-2]];//get the channelvalue update from host
	char channelRead[5];
	itoa(channelNumber[count - 2], channelRead, 10);
	sensorPingPayload.set_str(channelRead);//填读的通道号
	sensorPingPayload.set_str(";");
	char channelval[10];
	dtostrf(channelValue, 1, 2, channelval);
	sensorPingPayload.set_str(channelval);//填读的通道值
	sensorPingPayload.set_str("$");
#pragma endregion
	//发布
	_pubsubCommHelper->publish(_publishTopic, &sensorPingPayload, QOS1);

}

void CFETdeviceCommunicator::runWrite(Host* host)
{
	//调用_pubsubCommHelper的收发消息处理函数，将收到的报文从Xbee报文中提取MQTT-S报文，再从MQTT-S报文提取CFET device 协议报文
	_pubsubCommHelper->sendRecvMsg();
	_receiveDataPtr = (char*)_pubsubCommHelper->getReceiveDataPtr();//此时receiveDataPtr存的是CFET device 协议报文的初始地址
	_receiveDataLength = _pubsubCommHelper->getReceiveDatalength();//此时receiveDatalength存的是CFET device 协议报文的长度
	auto i = 0;
	unsigned int count = 0;
	String fieldString = "";
	unsigned int channelNumber[50];//存要写的通道号
	float channelValue[50];//存要写的通道值，与上面的通道号一一对应
	//从收到的消息中读出要写的通道号和通道值
	while (i<_receiveDataLength)
	{
		if ((_receiveDataPtr[i] == ';') || (_receiveDataPtr[i] == '$'))
		{
			//第一段是“SET”，忽略
			if (count == 0)
			{
				if (fieldString == "SET")
				{
					//字段字符串回归初始值
					fieldString = "";
					//字段计数+1
				}
				else
				{
					break;
				}
			}
			//除了第一段,从第二段开始，偶数段（对应count为奇数）是通道号，奇数段（对应count为偶数）是通道值
			else
			{
				if ((count&1)==0)//偶数count
				{
					//取出该字段转成float，这个float就是要写的通道值
					channelValue[(count>>1) - 1] = fieldString.toFloat();
				}
				else
				{
					//取出该字段转成int，这个int就是要用的通道号
					channelNumber[count>>1] = fieldString.toInt();
				}
				//字段字符串回归初始值
				fieldString = "";	
			}
			//字段计数+1
			count++;
			i++;
		}
		else
		{
			//如果不是分隔符“；”或者结束符“$”，把数据字符放进字段字符串里
			fieldString += _receiveDataPtr[i];
			i++;
		}
	}
	//更新host中相应的通道
	for (unsigned int i = 0; i < (count>>1); i++)//count除以2取商为要写的通道数量
	{
		host->ChannelSamples[channelNumber[i]] = channelValue[i];
	}
	//生成回复写指令的publish报文
#pragma region WriteResponsePub generation
	Payload WriteResponsePub = Payload(69);
	//报文头
	WriteResponsePub.set_str("PUB");
	WriteResponsePub.set_str(";");
	//写指令回复标志
	WriteResponsePub.set_str("SET");
	WriteResponsePub.set_str(";");
	//报文主体
	WriteResponsePub.set_str("SUCCESS");
	WriteResponsePub.set_str("$");
#pragma endregion
	//发布
	_pubsubCommHelper->publish(_publishTopic, &WriteResponsePub, QOS1);

}

