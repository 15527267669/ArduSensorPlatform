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
	//������ƽ̨����,max=4
	char* slaveName;
	//������ƽ̨ͨ������,6
	char* channelDescrip;
	//
	Pub_SubCommHelperBase* Pub_SubCommHelper;
	//����topic
	char* discTopic;
	//ע��topic
	char* regisTopic;
	//ע��ȷ��topic,max=6
	char* regisConfirmTopic;
	//������ƽ̨����topic,max=6
	char* publishTopic;
	//����������Topic,6
	char* sensorPingTopic;
	//CFET����Topic,6
	char* cfetPingTopic;
	//CFET��ͨ��topic,6
	char* cfetReadTopic;
	//CFETдͨ��topic,6
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
	//host
	Host* _host;
	unsigned long _sensorPingTimer;
	//CFET ״̬
	bool _cfetStatus= true;
	//CFET �����������ʱ��������λms
	unsigned long _maxPingInterval;
	//���µĴ�����ͨ��,��ʼֵ-1��ʾû����Ҫ���µ�ͨ��
	int _ChannelUpdated=-1;
	//�յ���CFET������ʼ��ַ
	char* _receiveDataPtr;
	//�յ���CFET���ĳ���
	uint8_t  _receiveDataLength;



	//check sensor whether online
	void checkOnline();
	//check sensor channelvalue updated or not 
	void checkChUpdated(Host * host);
	////�����յ���publish����
	//void analyzeReceiveData();
	void runRead(Host* host);
	void runWrite(Host* host);

	

	
};




#endif

