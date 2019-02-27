/*
 Name:		AutoTempHummMonitor.ino
 Created:	4/17/2018 8:29:40 PM
 Author:	jtext103
*/

#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include<TH10S_TempHum485.h>
#include<AutoTempHummCaculator.h>
#include<FanswitchPin.h>
#include <ModbusRTUComm.h>
#include <Timer.h>

//static host declearation, but host created in setupSensor are recommanded
Host host(4);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;

Timer* sensorTimer;
Timer* commTimer;
Timer* hummTimer;
int beyondtimes = 0;//��¼��������95%�Ĵ���
float *initValues = NULL;
// the setup function runs once when you press reset or power the board
void setup() {
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(1000);
	commTimer = new Timer(500);
	hummTimer = new Timer(5000);//���60s��hummֵ�Ƿ��Դ��ڻ�С��95%
	//if you donot need to init tha channels, comment the line below
	initAllChanells();

	//start, timer may need a start function
	host.Start(initValues);
	if (initValues != NULL)
	{
		delete[] initValues;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	//control loop
	if (sensorTimer->OnTime())
	{
		host.Update();
	}
	//it may not necessary to use another time base for comm
	if (commTimer->OnTime())
	{
		comm->HandleCommunication(&host);
	}
	//control the fan
	fanControl();

}

//set up sensor
void setupSensors()
{
	//construct our sensors and add them to host
	SensorBase* sensor;
	sensor = new TH10S_TempHum485(&Serial3, 9600, 1);
	host.AddSensor(sensor);
	sensor = new TH10S_TempHum485(&Serial3, 9600, 2);
	host.AddSensor(sensor);
	//sensor->Write(2, 2, true);
	sensor = new FanswitchPin(9,&host,4);
	host.AddSensor(sensor);
	sensor = new AutoTempHummCaculator(&host, 0, 1, 3, 4);//0~���¶ȣ�1��ʪ�ȣ�3���¶ȣ�4��ʪ��
	host.AddSensor(sensor);
}

void initAllChanells()
{
	initValues = new float[10]();
	for (size_t i = 0; i < 10; i++)
	{
		initValues[i] = 0;
	}
}

//set up modbus communication
void setupComm()
{
	//normally you have more things to do tosetup a proper communication
	//like port, speed, address and etc.
	//  comm = new ModbusRTUComm(&Serial, &Serial, 9600, 2);
	comm = new ModbusRTUComm(&Serial1, 9600, 2);
}
//�����ȴ��翪��
void fanControl()
{
	if (hummTimer->OnTime())
	{
		if (host.ChannelSamples[6] == 1)//7ͨ��Ϊʪ�ȼ��ͨ�������ʪ��Ϊ�ڲ�ʪ��
		{
			beyondtimes++;
		}
		else 
		{
			beyondtimes--;
		}
	}
	//if ((host.ChannelSamples[6] == 1) && (hummTimer->OnTime()))//7ͨ��Ϊʪ�ȼ��ͨ�������ʪ��Ϊ�ڲ�ʪ��
	//{
	//	beyondtimes++;
	//}
	//else if (hummTimer->OnTime())
	//{
	//	beyondtimes--;
	//}
	//���������μ�⵽fan'switchPINΪ1ʱ�����ȴ��翪�ش򿪣������ʱ��������hummTimer ��ʱ������
	if (beyondtimes >= 3)
	{
		beyondtimes = 3;
		digitalWrite(9, HIGH);//�ȷ����������Ϊ9
	}
	else
	{
		digitalWrite(9, LOW);//�ȷ����������Ϊ9
	}
	
}