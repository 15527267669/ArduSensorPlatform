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

//����ָ���Ǹ���topic�����֣�������payload
typedef struct {
	//������ƽ̨����
	char* slaveName;
	//������ƽ̨ͨ������
	char* channelDescrip;
	//����topic
	char* discTopic;
	//ע��topic
	char* regisTopic;
	//ע��ȷ��topic
	char* regisConfirmTopic;
	//������ƽ̨����topic
	char* publishTopic;
	//����������Topic
	char* sensorPingTopic;
	//CFET����Topic
	char* cfetPingTopic;
	//CFET��ͨ��topic
	char* cfetReadTopic;
	//CFETдͨ��topic
	char* cfetWriteTopic;
}CfetDeviceCommConfig;

class CFETdeviceCommunicator:public CommunicationBase
{
public:
	CFETdeviceCommunicator();
	CFETdeviceCommunicator(CfetDeviceCommConfig* config);
	~CFETdeviceCommunicator();

	virtual int HandleCommunication(Host* host) override;
	//���ô�����ͨ��ֵ����״ֵ̬�����ⲿ�ӿں���
	void setUpdateChannel(uint8_t updateChannel);

	//װ��Pub_SubCommHelper���͵�ʵ����Ҳ�����������������ʵ����,��������pubsubCommhelper�Ǹ�����Ҫ��̬������һ��ָ�룬��ʹ��ʱ�����ô������
	void LoadCommhelper(Pub_SubCommHelperBase* pubsubCommhelper);
	//CFETdeviceComm������������ƽ̨���ķ���Topic
	void start();
	//����Topic�ص��������յ�����Topic������
	int discCallback(MqttsnPublish* registerMsg);
	//ע��ȷ��Topic�ص��������յ�ע��ȷ��Topic������
	int regisConfirmCallback(MqttsnPublish* registerMsg);
	//cfet����topic�ص��������յ�cfet����topic������
	int cfetpingCallback(MqttsnPublish* registerMsg);
	//cfet��topic�ص��������յ�cfet��topic������
	int cfetReadCallback(MqttsnPublish* registerMsg);
	//cfetдtopic�ص��������յ�cfetдtopic������
	int	cfetWriteCallback(MqttsnPublish* registerMsg);

private:
	//Pub_SubCommHelper pointer for the need to set up Pub_SubComm ;Pub_SubCommHelper���͵�ָ�룬Ϊ��ʹ��Pub_SubCommHelperBase�Ľӿں���������
	Pub_SubCommHelperBase* _pubsubCommHelper;
	//������ƽ̨����
	MQString* _slaveName;
	//������ƽ̨ͨ������
	MQString* _channelDescrip;
	//����topic
	MQString* _discTopic;
	//ע��topic
	MQString* _regisTopic;
	//ע��ȷ��topic
	MQString* _regisConfirmTopic;
	//������ƽ̨����topic
	MQString* _publishTopic;
	//����������Topic
	MQString* _sensorPingTopic;
	//CFET����Topic
	MQString* _cfetPingTopic;
	//CFET��ͨ��topic
	MQString* _cfetReadTopic;
	//CFETдͨ��topic
	MQString*_cfetWriteTopic;
	//Sensor������ʱ��
	unsigned long _sensorPingTimer;
	//CFET ״̬
	bool _cfetStatus= true;
	//CFET �����������ʱ��������λms
	unsigned long _maxPingInterval;
	//���µĴ�����ͨ��,��ʼֵ-1��ʾû����Ҫ���µ�ͨ��
	int _ChannelUpdated=-1;



	//check sensor whether online
	void checkOnline();
	//check sensor channelvalue updated or not 
	void checkChUpdated(Host * host);

	

	
};




#endif

