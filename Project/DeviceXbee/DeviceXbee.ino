/*
 Name:		DeviceXbee.ino
 Created:	1/20/2018 10:40:46 PM
 Author:	jtext103
*/



#include <MAX31855_ThermoSensor.h>
#include <PWM_PinSensor.h>
#include <SPI.h>
#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include <Timer.h>
#include <DigitalPin.h>
#include <ModbusCommXbeeInterface.h>
#ifndef USBCON

//FOR MEGA 2560
////define OUTPUT to H_bridge PIN
#define PWM1  8        //set 8 pin is PWM1 output
#define PWM2  9        //set 9 pin is PWM2 output
#define DIR1  30       //set 30 pin is dir1 output
#define DIR2  31       //set 31 pin is dir2 output

#define SSR  32       //set 32 pin is SSR output
//define SPI PIN
#define MAXDO  50        //set 50 pin is MISO
//#define MAXDO2  51        //set 51 pin is MISO2 
#define MAXCLK  52       //set 52 pin is SCLK 
//#define MAXCLK2  53       //set 53 pin is SCLK2 
#define MAXCS1  34       //set 34 pin is CS1 
#define MAXCS2  35       //set 35 pin is CS2
#else
//FOR DFROBOT leonardo
//define OUTPUT to H_bridge PIN
#define PWM1  10        //set 8 pin is PWM1 output
#define PWM2  11        //set 9 pin is PWM2 output
#define DIR1  12       //set 30 pin is dir1 output
#define DIR2  13       //set 31 pin is dir2 output

#define SSR  8       //set 32 pin is SSR output
//define SPI PIN
#define MAXDO  7        //set 50 pin is MISO
//#define MAXDO2  51        //set 51 pin is MISO2 
#define MAXCLK  6       //set 52 pin is SCLK 
//#define MAXCLK2  53       //set 53 pin is SCLK2 
#define MAXCS1  5       //set 34 pin is CS1 
#define MAXCS2  4       //set 35 pin is CS2

#endif // !USBCON

////static host declearation, but host created in setupSensor are recommanded
//Host host(5);
//Timer* sensorTimer;
//Timer* commTimer;
//float *initValues = NULL;
//
////******************
////create the  XBee object.
//XBee device_xbee = XBee();
//XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A6812D);
//uint16_t addr16 = 0x8ED0;
//uint8_t broadcastRadius = 0;
//uint8_t option = 0;
//uint8_t frameId = 0x01;
//
//// create reusable response objects for responses we expect to handle 
//ZBRxResponse rx = ZBRxResponse();
//ModemStatusResponse msr = ModemStatusResponse();
////create a byte array storage the data from received API message
//uint8_t* dataFrame=NULL;
//uint8_t dataFrameLength = 0;
//
//
////set TX indicator LED at Pin 13 to show TX status and TX response status
//int tx_statusLed = 13;
//int tx_errorLed = 13;
//
////set RX indicator LED at Pin 13 to show RX status and RX response status
//int rx_statusLed = 12;
//int rx_errorLed = 12;
////********************
//
//void setup()
//{
//	//setup all sensors
//	setupSensors();
//	//setup timers
//	sensorTimer = new Timer(500);
//	commTimer = new Timer(3000);
//
//	//if you donot need to init tha channels, comment the line below
//	initAllChanells();
//
//	//start, timer may need a start function
//	host.Start(initValues);
//	if (initValues != NULL)
//	{
//		delete[] initValues;
//	}
//	//*****************
//	pinMode(tx_statusLed, OUTPUT);
//	pinMode(tx_errorLed, OUTPUT);
//	pinMode(rx_statusLed, OUTPUT);
//	pinMode(rx_errorLed, OUTPUT);
//	Serial.begin(9600);
//	Serial1.begin(9600);
//	//set Serial1 to xbee module
//	device_xbee.setSerial(Serial1);
//	//*****************
//}
//
//// the loop function runs over and over again until power down or reset
//void loop()
//{
//	//control loop
//	if (sensorTimer->OnTime())
//	{
//		host.Update();
//	}
//	//it may not necessary to use another time base for comm
//	if (commTimer->OnTime())
//	{
//		handleCommunication(&host);
//	}
//}
//
////set up sensor
//void setupSensors()
//{
//	//construct our sensors and add them to host
//	SensorBase* sensor;
//	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS1, MAXDO);
//	host.AddSensor(sensor);
//	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS2, MAXDO);
//	host.AddSensor(sensor);
//	sensor = new PWM_PinSensor(PWM1, DIR1);
//	host.AddSensor(sensor);
//	sensor = new PWM_PinSensor(PWM2, DIR2);
//	host.AddSensor(sensor);
//	sensor = new DigitalPin(SSR, true);
//	host.AddSensor(sensor);
//}
//
//void initAllChanells()
//{
//	initValues = new float[5]();
//	for (size_t i = 0; i < 5; i++)
//	{
//		initValues[i] = 0;
//	}
//}
////the Led_flash function
//void flashLed(int pin, int times, int wait)
//{
//
//	for (int i = 0; i < times; i++)
//	{
//		digitalWrite(pin, HIGH);
//		delay(wait);
//		digitalWrite(pin, LOW);
//
//		if (i + 1 < times)
//		{
//			delay(wait);
//		}
//	}
//}
////communication
//void handleCommunication(Host * host)
//{
//	//receive RX packet or MODEM status response packet
//	rx_Comm();
//	//send inquire result packet
//	tx_Comm(host);
//}
////receive inqire order or MODEM status response packet
//void rx_Comm()
//{
//	//receive RX packet
//	device_xbee.readPacket();
//
//	if (device_xbee.getResponse().isAvailable()) {
//		//Serial.println("GET");
//		// got something
//
//		if (device_xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
//			// got a zb rx packet
//			//Serial.write(device_xbee.getResponse().getApiId());
//			// now fill our zb rx class
//			device_xbee.getResponse().getZBRxResponse(rx);
//
//			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
//				// the sender got an ACK
//				//Serial.write(rx.getOption());
//				flashLed(rx_statusLed, 10, 10);
//			}
//			else {
//				// we got it (obviously) but sender didn't get an ACK
//				flashLed(rx_errorLed, 2, 20);
//			}
//			// get the data from RX packet
//			dataFrameLength=getDataFrame();
//			/*for (size_t i = 0; i < dataFrameLength; i++)
//			{
//				Serial.write(dataFrame[i]);
//			}*/
//		}
//		else if (device_xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
//			device_xbee.getResponse().getModemStatusResponse(msr);
//			// the local XBee sends this response on certain events, like association/dissociation
//
//			if (msr.getStatus() == ASSOCIATED) {
//				// this is great.  flash led
//				flashLed(rx_statusLed, 10, 10);
//			}
//			else if (msr.getStatus() == DISASSOCIATED) {
//				// this is awful.. flash led to show our discontent
//				flashLed(rx_errorLed, 10, 10);
//			}
//			else {
//				// another status
//				flashLed(rx_statusLed, 5, 10);
//			}
//		}
//		else {
//			// not something we were expecting
//			flashLed(rx_errorLed, 1, 25);
//		}
//	}
//	else if (device_xbee.getResponse().isError()) {
//		Serial.print("Error reading packet.  Error code: ");
//		Serial.println(device_xbee.getResponse().getErrorCode());
//	}
//}
////storage the data from received API message and return the dataLength
//uint8_t getDataFrame()
//{
//	dataFrame = rx.getData();
//	return rx.getDataLength();
//}
////recognize the data and make a response
//void tx_Comm(Host * host)
//{
//	//recognize the data if right inquire order
//	/*//check address
//	if (dataFrame[0] != _slaveId)
//	{
//		return;
//	}
//	*/
//	//check check sum
//	if (dataFrame==NULL)
//	{
//		return;
//	}
//	if (!checkSumOk(dataFrame))
//	{
//		//_port->println('b');//checkSumOk is wrong
//		return;
//	}
//	//_port->println('a');//checkSumOk is true
//	//check function code
//	if (dataFrame[1] != 3 && dataFrame[1] != 16)
//	{
//		//Serial.println('c');
//		return;
//	}
//	//determin command type
//	switch (dataFrame[1])
//	{
//		//run the command
//	case 3:
//	{
//		//Serial.println("read");
//		readResponse(host, dataFrame);
//		dataFrame = NULL;
//	}
//	break;
//	case 16:
//	{
//		writeResponse(host, dataFrame);
//		dataFrame = NULL;
//	}
//	}
//	return;
//}
//bool checkSumOk(byte * frame)
//{
//	unsigned int calcCheckSum = 0;
//	for (byte i = 0; i < dataFrameLength - 2; i++)
//	{
//		calcCheckSum += frame[i];
//	}
//	unsigned int receivedCRC;
//	unsigned char  _checkSumBytes[2];
//	/*for (byte i = _frameLength - 2; i < _frameLength; i++)
//	{
//	_checkSumBytes[0] = frame[i];
//	}*/
//	_checkSumBytes[1] = frame[dataFrameLength - 2];
//	_checkSumBytes[0] = frame[dataFrameLength - 1];
//
//	Byte_to_Uint(&receivedCRC, _checkSumBytes);
//	if (calcCheckSum != receivedCRC)
//	{
//		return false;
//	}
//	return true;
//}
//void readResponse(Host*_host, uint8_t*_dataFrame)
//{
//	unsigned int _u16ReadChannelAddress;
//	unsigned int _u16ReadChannelQty;
//	//get start channel, and channel numbers to read
//	unsigned int _u16RequestBuffer[2];
//	for (byte i = 0; i < 2; i++)
//	{
//		_u16RequestBuffer[i] = word(_dataFrame[2 * i + 2], _dataFrame[2 * i + 3]);
//	}
//	_u16ReadChannelAddress = _u16RequestBuffer[0]; //use _u16RequestBuffer[0] storage 16bit ChannelAddress
//	_u16ReadChannelQty = _u16RequestBuffer[1];     //use _u16RequestBuffer[1] storage 16bit ChannelQty
//												   //make a reply frame
//	float _32TransmitBuffer[64];
//	for (byte i = 0; i < _u16ReadChannelQty; i++)
//	{
//		/* use _32TransmitBuffer[i] storage channelsamples[i] for preparing
//		subsequent disassembling channelsample to transmit
//		*/
//		_32TransmitBuffer[i] = _host->ChannelSamples[_u16ReadChannelAddress + i];
//
//	}
//	byte u8ModbusTransmitADU[256];
//	byte u8ModbusTransmitADUSize = 0;
//	u8ModbusTransmitADU[0] = 2;
//	u8ModbusTransmitADU[1] = 3;
//	u8ModbusTransmitADU[2] = 4 * _u16ReadChannelQty;
//	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 3;
//	
//	//disassembling channelsample from float to byte[] and fill into u8ModbusTransmitADU[]
//	for (size_t i = 0; i <_u16ReadChannelQty; i++)
//	{
//		unsigned char channelsampleByte[4];
//		Float_to_Byte(_32TransmitBuffer[i], channelsampleByte);
//		//Highbyte forward!!
//		u8ModbusTransmitADU[4 * i + 3] = channelsampleByte[3];
//		u8ModbusTransmitADU[4 * i + 4] = channelsampleByte[2];
//		u8ModbusTransmitADU[4 * i + 5] = channelsampleByte[1];
//		u8ModbusTransmitADU[4 * i + 6] = channelsampleByte[0];
//		u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 4;
//	}
//
//	//Serial.println("sendready");
//	sendResultAndCheck(u8ModbusTransmitADUSize, u8ModbusTransmitADU);
//}
//
//void writeResponse(Host*_host, uint8_t*_dataFrame)
//{
//	unsigned int _u16WriteChannelAddress;
//	unsigned int _u16WriteChannelQty;
//	uint8_t u8Qty;
//	//for each channel to write
//	unsigned int _u16RequestBuffer[2];
//	for (byte i = 0; i < 2; i++)
//	{
//		_u16RequestBuffer[i] = word(_dataFrame[2 * i + 2], _dataFrame[2 * i + 3]);//Highbyte foward
//	}
//	_u16WriteChannelAddress = _u16RequestBuffer[0]; //set WriteChannelAddress and WriteChannelQty
//	_u16WriteChannelQty = _u16RequestBuffer[1];
//	u8Qty = _dataFrame[6];
//	//disassemble Payload
//	for (byte i = 0; i < _u16WriteChannelQty; i++)
//	{
//		unsigned char chanNo[50];
//		float chanSample[50];
//		unsigned char sampleByte[4];
//		float* sample;
//		chanNo[i] = _dataFrame[7 + 5 * i];
//		//Highbyte forward!
//		sampleByte[3] = _dataFrame[8 + 5 * i];
//		sampleByte[2] = _dataFrame[9 + 5 * i];
//		sampleByte[1] = _dataFrame[10 + 5 * i];
//		sampleByte[0] = _dataFrame[11 + 5 * i];
//		Byte_to_Float(sample, sampleByte);
//		chanSample[i] = *sample;
//		//write channelSample to corresponding channel in host
//		_host->ChannelSamples[chanNo[i]] = chanSample[i];
//
//		/*Serial.println(chanSample[i]);*/
//	}
//	//make a reply frame
//	byte u8ModbusTransmitADU[256];
//	byte u8ModbusTransmitADUSize = 0;
//	u8ModbusTransmitADU[0] = 2;
//	u8ModbusTransmitADU[1] = 16;
//	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
//	u8ModbusTransmitADU[2] = _dataFrame[2];         //WriteChannelAddress
//	u8ModbusTransmitADU[3] = _dataFrame[3];
//	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
//	u8ModbusTransmitADU[4] = _dataFrame[4];        //WriteChannelQty
//	u8ModbusTransmitADU[5] = _dataFrame[5];
//	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
//	sendResultAndCheck(u8ModbusTransmitADUSize, u8ModbusTransmitADU);
//}
//void sendResultAndCheck(byte u8ModbusTransmitADUSize, byte* u8ModbusTransmitADU)
//{
//	// calculate CRC
//	unsigned int transCRC = 0;
//	for (byte i = 0; i < u8ModbusTransmitADUSize; i++)
//	{
//		transCRC += u8ModbusTransmitADU[i];
//	}
//	//fill  CRC into u8ModbusTransmitADU[]
//	unsigned char crcByte[2];
//	Uint_to_Byte(transCRC, crcByte);
//	
//	u8ModbusTransmitADU[u8ModbusTransmitADUSize++] = crcByte[1];//Highbyte forward!!
//	u8ModbusTransmitADU[u8ModbusTransmitADUSize++] = crcByte[0];
//	/*Serial.write(u8ModbusTransmitADUSize);*/
//	/*for (size_t i = 0; i < u8ModbusTransmitADUSize; i++)
//	{
//		Serial.write(u8ModbusTransmitADU[i]);
//	}*/
//	// send TX packet including inquire result***************
//	//create a TX request ,config parameters of the destination node
//	//ZBTxRequest zbTx = ZBTxRequest(addr64, addr16, broadcastRadius, option,u8ModbusTransmitADU, u8ModbusTransmitADUSize, frameId);
//	ZBTxRequest zbTx = ZBTxRequest(addr64, u8ModbusTransmitADU, u8ModbusTransmitADUSize);
//	// create a TX response for loading the response of TX request------to get the status of TX messege
//	ZBTxStatusResponse txStatus = ZBTxStatusResponse();
//
//	//send the TX request message
//	device_xbee.send(zbTx);
//	/*Serial1.write(0x7e);
//	Serial1.write(0x00);
//	Serial1.write(0x16);
//	Serial1.write(0x10);
//	Serial1.write(0x01);
//	Serial1.write(0x00);
//	Serial1.write(0x13);
//	Serial1.write(0xa2);
//	Serial1.write(0x00);
//	Serial1.write(0x40);
//	Serial1.write(0xa6);
//	Serial1.write(0x81);
//	Serial1.write(0x2d);
//	Serial1.write(0x8e);
//	Serial1.write(0xd0);
//	Serial1.write(0x00); 
//	Serial1.write(0x00);
//	Serial1.write(0x02);
//	Serial1.write(0x03);
//	Serial1.write(0x00);
//	Serial1.write(0x00);
//	Serial1.write(0x00);
//	Serial1.write(0x05);
//	Serial1.write(0x00);
//	Serial1.write(0x0a);
//	Serial1.write(0x33);*/
//	
//	for (size_t i = 0; i < u8ModbusTransmitADUSize; i++)
//	{
//	Serial1.write(u8ModbusTransmitADU[i]);
//	}
//	// flash TX indicator LED to show having already send the TX request message
//	flashLed(tx_statusLed, 1, 100);
//
//	// after sending a tx request, we expect a status response
//	// wait up to half second for the status response
//	if (device_xbee.readPacket(500))
//	{
//		// got a response!
//
//		// should be a znet tx status            	
//		if (device_xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
//		{
//			device_xbee.getResponse().getZBTxStatusResponse(txStatus);
//
//			// get the delivery status, the fifth byte
//			if (txStatus.getDeliveryStatus() == SUCCESS)
//			{
//				// success!!---  time to celebrate
//				flashLed(tx_statusLed, 5, 50);
//			}
//			else
//			{
//				// failure£¡£¡£¡---the remote XBee did not receive our packet. is it powered on?
//				flashLed(tx_errorLed, 3, 500);
//			}
//		}
//	}
//	else if (device_xbee.getResponse().isError())
//	{
//		Serial.print("Error reading packet.  Error code: ");
//		Serial.println(device_xbee.getResponse().getErrorCode());
//	}
//	else
//	{
//		// local XBee did not provide a timely TX Status Response -- should not happen
//		flashLed(tx_errorLed, 2, 50);
//	}
//
//	delay(1000);
//}



//static host declearation, but host created in setupSensor are recommanded
Host host(5);
//different as host setup, the communication is set up dunamically
CommunicationBase* comm;
Timer* sensorTimer;
Timer* commTimer;
float *initValues = NULL;

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A6812D);
uint16_t addr16 = 0x8ED0;
//set TX indicator LED at Pin 13 to show TX status and TX response status
int tx_statusLed = 13;
int tx_errorLed = 13;

//set RX indicator LED at Pin 13 to show RX status and RX response status
int rx_statusLed = 12;
int rx_errorLed = 12;
// the setup function runs once when you press reset or power the board
void setup()
{
	//setup all sensors
	setupSensors();
	//setup communications
	setupComm();
	//setup timers
	sensorTimer = new Timer(500);
	commTimer = new Timer(10);

	//if you donot need to init tha channels, comment the line below
	initAllChanells();

	//start, timer may need a start function
	host.Start(initValues);
	if (initValues != NULL)
	{
		delete[] initValues;
	}
		pinMode(tx_statusLed, OUTPUT);
		pinMode(tx_errorLed, OUTPUT);
		pinMode(rx_statusLed, OUTPUT);
		pinMode(rx_errorLed, OUTPUT);
}
// the loop function runs over and over again until power down or reset
void loop()
{
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
}
//set up sensor
void setupSensors()
{
	//construct our sensors and add them to host
	SensorBase* sensor;
	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS1, MAXDO);
	host.AddSensor(sensor);
	sensor = new MAX31855_ThermoSensor(MAXCLK, MAXCS2, MAXDO);
	host.AddSensor(sensor);
	sensor = new PWM_PinSensor(PWM1, DIR1);
	host.AddSensor(sensor);
	sensor = new PWM_PinSensor(PWM2, DIR2);
	host.AddSensor(sensor);
	sensor = new DigitalPin(SSR, true);
	host.AddSensor(sensor);
}

void initAllChanells()
{
	initValues = new float[5]();
	for (size_t i = 0; i < 5; i++)
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
	comm = new ModbusXbeeComm(&Serial, &Serial1, 9600, 2, addr64, addr16);
}