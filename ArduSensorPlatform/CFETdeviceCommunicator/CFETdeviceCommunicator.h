/*
 Name:		CFETdeviceCommunicator.h
 Created:	2019/3/14 11:24:56
 Author:	jtext103
 Editor:	http://www.visualmicro.com
*/

#ifndef _CFETdeviceCommunicator_h
#define _CFETdeviceCommunicator_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include<stdlib.h>
#include<CommunicationBase.h>
#include<Pub_SubPatternCommHelper.h>

//这里指的是各个topic的名字，而不是payload
typedef struct {
	//传感器平台名字,max=4
	char* slaveName;
	//传感器平台通道描述,6
	char* channelDescrip;
	//
	Pub_SubCommHelperBase* Pub_SubCommHelper;
	//发现topic
	char* discTopic;
	//注册topic
	char* regisTopic;
	//注册确认topic,max=6
	char* regisConfirmTopic;
	//传感器平台发布topic,max=6
	char* publishTopic;
	//传感器心跳Topic,6
	char* sensorPingTopic;
	//CFET心跳Topic,6
	char* cfetPingTopic;
	//CFET读通道topic,6
	char* cfetReadTopic;
	//CFET写通道topic,6
	char* cfetWriteTopic;
}CfetDeviceCommConfig;

class CFETdeviceCommunicator:public CommunicationBase
{
public:
	CFETdeviceCommunicator();
	CFETdeviceCommunicator(CfetDeviceCommConfig* config);
	~CFETdeviceCommunicator();

	virtual int HandleCommunication(Host* host) override;
	//设置传感器通道值更新状态值——外部接口函数
	void setUpdateChannel(uint8_t updateChannel);

	//装载Pub_SubCommHelper类型的实例（也可能是它的派生类的实例）,函数参数pubsubCommhelper是根据需要动态建立的一个指针，在使用时被调用传入参数
	void LoadCommhelper(Pub_SubCommHelperBase* pubsubCommhelper);
	//CFETdeviceComm启动，传感器平台订阅发现Topic
	void start();
	//发现Topic回调函数，收到发现Topic后运行
	int discCallback(MqttsnPublish* registerMsg);
	//注册确认Topic回调函数，收到注册确认Topic后运行
	int regisConfirmCallback(MqttsnPublish* registerMsg);
	//cfet心跳topic回调函数，收到cfet心跳topic后运行
	int cfetpingCallback(MqttsnPublish* registerMsg);
	//cfet读topic回调函数，收到cfet读topic后运行
	int cfetReadCallback(MqttsnPublish* registerMsg);
	//cfet写topic回调函数，收到cfet写topic后运行
	int	cfetWriteCallback(MqttsnPublish* registerMsg);

private:
	//Pub_SubCommHelper pointer for the need to set up Pub_SubComm ;Pub_SubCommHelper类型的指针，为了使用Pub_SubCommHelperBase的接口函数而留存
	Pub_SubCommHelperBase* _pubsubCommHelper;
	//传感器平台名称
	MQString* _slaveName;
	//传感器平台通道描述
	MQString* _channelDescrip;
	//发现topic
	MQString* _discTopic;
	//注册topic
	MQString* _regisTopic;
	//注册确认topic
	MQString* _regisConfirmTopic;
	//传感器平台发布topic
	MQString* _publishTopic;
	//传感器心跳Topic
	MQString* _sensorPingTopic;
	//CFET心跳Topic
	MQString* _cfetPingTopic;
	//CFET读通道topic
	MQString* _cfetReadTopic;
	//CFET写通道topic
	MQString*_cfetWriteTopic;
	//Sensor心跳计时器
	//host
	Host* _host;
	unsigned long _sensorPingTimer;
	//CFET 状态
	bool _cfetStatus= true;
	//CFET 心跳连接最大时间间隔，单位ms
	unsigned long _maxPingInterval;
	//更新的传感器通道,初始值-1表示没有需要更新的通道
	int _ChannelUpdated=-1;
	//收到的CFET报文起始地址
	char* _receiveDataPtr;
	//收到的CFET报文长度
	uint8_t  _receiveDataLength;



	//check sensor whether online
	void checkOnline();
	//check sensor channelvalue updated or not 
	void checkChUpdated(Host * host);
	////解析收到的publish报文
	//void analyzeReceiveData();
	void runRead(Host* host);
	void runWrite(Host* host);

	

	
};




#endif

