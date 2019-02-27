#include "ModbusRTUComm.h"

//ModbusRTUComm::ModbusRTUComm(MySerial * RXport, HardwareSerial * Txport, long baud, byte slaveId)
ModbusRTUComm::ModbusRTUComm(MySerial * RXport, long baud, byte slaveId)
{
	this->_Rxport = RXport;
	(*RXport).begin(baud);
	/*this->_Txport = Txport;
	(*Txport).begin(baud);*/

	_slaveId = slaveId;

	delay(2000);

	if (baud > 19200) {
		_t15 = 750;
		_t35 = 1750;
	}
	else {
		_t15 = 15000000 / baud; // 1T * 1.5 = T1.5
		_t35 = 35000000 / baud; // 1T * 3.5 = T3.5
	}
}

ModbusRTUComm::~ModbusRTUComm()
{
}

int ModbusRTUComm::HandleCommunication(Host * host)
{
	//recieve a frame
	

	while ((*_Rxport).available() > _frameLength) {
		_frameLength = (*_Rxport).available();
		delayMicroseconds(_t15);
	}
	//if no data recieved just return 
	if (_frameLength == 0) return;

	byte i;
	auto _frame = new byte[_frameLength];
	for (i = 0; i < _frameLength; i++) _frame[i] = (*_Rxport).read();

	
	//process the frame
	processFrame(host,_frame);

	delete[] _frame;
	_frameLength = 0;
	
	
	return 0;
}

void  ModbusRTUComm::processFrame(Host * host,byte* frame)
{
	//check address
	if (frame[0] != _slaveId)
	{
		return;
	}

	//check check sum
	if (!checkSumOk(frame))
	{
		//_port->println('b');//checkSumOk is wrong
		return;
	}
	//_port->println('a');//checkSumOk is true
	//check function code
	if (frame[1] != 3&& frame[1] != 16)
	{
		//_port->println('c');
		return;
	}
	//determin command type
	switch (frame[1])
	{
		//run the command
		case 3:
		{
			runRead(host, frame);
	     }
		break;
		case 16:
		{
			runWrite(host, frame);
		}
	}
	

	
	

}

bool ModbusRTUComm::checkSumOk(byte * frame)
{
	unsigned int calcCheckSum = 0;
	for (byte i = 0; i < _frameLength-2; i++)
	{
		calcCheckSum += frame[i];
	}
	unsigned int receivedCRC;
	unsigned char  _checkSumBytes[2];
	/*for (byte i = _frameLength - 2; i < _frameLength; i++)
	{
		_checkSumBytes[0] = frame[i];
	}*/
	_checkSumBytes[1] = frame[_frameLength - 2];
	_checkSumBytes[0] = frame[_frameLength - 1];

	Byte_to_Uint(&receivedCRC, _checkSumBytes);
	if (calcCheckSum!= receivedCRC)
	{
		return false;
	}
	return true;
}

void  ModbusRTUComm::runRead(Host * host, byte* frame)
{
	//get start channel, and channel numbers to read
	unsigned int _u16RequestBuffer[2];
	for (byte i = 0; i < 2; i++)
	{
		_u16RequestBuffer[i] = word(frame[2 * i + 2], frame[2 * i + 3]);
	}
	_u16ReadChannelAddress = _u16RequestBuffer[0]; //use _u16RequestBuffer[0] storage 16bit ChannelAddress
	_u16ReadChannelQty = _u16RequestBuffer[1];     //use _u16RequestBuffer[1] storage 16bit ChannelQty
	//make a reply frame
	float _32TransmitBuffer[64];
	for (byte i = 0; i < _u16ReadChannelQty; i++)
	{
		/* use _32TransmitBuffer[i] storage channelsamples[i] for preparing
		subsequent disassembling channelsample to transmit
		*/
		_32TransmitBuffer[i] = host->ChannelSamples[_u16ReadChannelAddress + i];
		
	}
	byte u8ModbusTransmitADU[256];
	byte u8ModbusTransmitADUSize = 0;
	u8ModbusTransmitADU[0] = _slaveId;
	u8ModbusTransmitADU[1] = 3;
	u8ModbusTransmitADU[2] = 4 * _u16ReadChannelQty;
	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 3;
	//disassembling channelsample from float to byte[] and fill into u8ModbusTransmitADU[]
	for (byte i = 0; i < _u16ReadChannelQty; i++)
	{
		unsigned char channelsampleByte[4];
		Float_to_Byte(_32TransmitBuffer[i], channelsampleByte);
		//Highbyte forward!!
		u8ModbusTransmitADU[4 * i + 3] = channelsampleByte[3];
		u8ModbusTransmitADU[4 * i + 4] = channelsampleByte[2];
		u8ModbusTransmitADU[4 * i + 5] = channelsampleByte[1];
		u8ModbusTransmitADU[4 * i + 6] = channelsampleByte[0];
		u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 4;
	}

	//calculate CRC and fill it into u8ModbusTransmitADU and send
	send(calcCRC(u8ModbusTransmitADUSize, u8ModbusTransmitADU), u8ModbusTransmitADU);
}

void  ModbusRTUComm::runWrite(Host * host, byte* frame)
{
	//for each channel to write
	unsigned int _u16RequestBuffer[2];
	for (byte i = 0; i < 2; i++)
	{
		_u16RequestBuffer[i] = word(frame[2 * i + 2], frame[2 * i + 3]);//Highbyte foward
	}
	_u16WriteChannelAddress = _u16RequestBuffer[0]; //set WriteChannelAddress and WriteChannelQty
	_u16WriteChannelQty = _u16RequestBuffer[1];
	u8Qty = frame[6];
	//disassemble Payload
	for (byte i = 0; i < _u16WriteChannelQty; i++)
	{
		unsigned char chanNo[50];
		float chanSample[50];
		unsigned char sampleByte[4];
		float* sample;
		chanNo[i] = frame[7 + 5 * i];
		//Highbyte forward!
		sampleByte[3] = frame[8 + 5 * i];
		sampleByte[2] = frame[9 + 5 * i];
		sampleByte[1] = frame[10 + 5 * i];
		sampleByte[0] = frame[11 + 5 * i];
		Byte_to_Float(sample, sampleByte);
		chanSample[i] = *sample;
		//write channelSample to corresponding channel in host
		host->ChannelSamples[chanNo[i]] = chanSample[i];
	
		/*Serial.println(chanSample[i]);*/
	}
	//make a reply frame
	byte u8ModbusTransmitADU[256];
	byte u8ModbusTransmitADUSize = 0;
	u8ModbusTransmitADU[0] = _slaveId;
	u8ModbusTransmitADU[1] = 16;
	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
	u8ModbusTransmitADU[2] = frame[2];         //WriteChannelAddress
	u8ModbusTransmitADU[3] = frame[3];
	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
	u8ModbusTransmitADU[4] = frame[4];        //WriteChannelQty
	u8ModbusTransmitADU[5] = frame[5];
	u8ModbusTransmitADUSize = u8ModbusTransmitADUSize + 2;
	
	//calculate CRC and fill it into u8ModbusTransmitADU and send
	send(calcCRC(u8ModbusTransmitADUSize, u8ModbusTransmitADU), u8ModbusTransmitADU);
}

//send reply function
void  ModbusRTUComm::send(byte u8ModbusTransmitADUSize, byte* u8ModbusTransmitADU)
{
	for (byte i = 0; i < u8ModbusTransmitADUSize; i++)
	{
		//_Txport->write(u8ModbusTransmitADU[i]);
		_Rxport->write(u8ModbusTransmitADU[i]);
	}
}

//send CRC calculate and fill function
//retrun the final frame length
int ModbusRTUComm::calcCRC(byte u8ModbusTransmitADUSize, byte* u8ModbusTransmitADU)
{
	// calculate CRC
	unsigned int transCRC = 0;
	for (byte i = 0; i < u8ModbusTransmitADUSize; i++)
	{
		transCRC += u8ModbusTransmitADU[i];
	}
	//fill  CRC into u8ModbusTransmitADU[]
	unsigned char crcByte[2];
	Uint_to_Byte(transCRC, crcByte);
	u8ModbusTransmitADU[u8ModbusTransmitADUSize++] = crcByte[1];//Highbyte forward!!
	u8ModbusTransmitADU[u8ModbusTransmitADUSize++] = crcByte[0];
	u8ModbusTransmitADU[u8ModbusTransmitADUSize] = 0;
	return u8ModbusTransmitADUSize;
}
