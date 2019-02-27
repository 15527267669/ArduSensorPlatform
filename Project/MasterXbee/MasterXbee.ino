/*
 Name:		MasterXbee.ino
 Created:	1/21/2018 2:34:53 PM
 Author:	jtext103
*/

/*
This example is for Series 2 XBee
Sends a Zigbee TX request and checks the status response for success
*/

#include <XBee.h>
#include <Printers.h>


//create the  XBee object.
XBee master_xbee = XBee();
//Load the payload---the actual content
uint8_t payload[] = { 0x02, 0x03,0x00,0x00,0x00,0x05,0x00,0x0A }; //按照自定义modbus协议，查询0~4五个通道值
																  // SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A89777);

//create a TX request ,config parameters of the destination node
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
// create a TX response for loading the response of TX request------to get the status of TX messege
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


//set TX indicator LED at Pin 13 to show TX status and TX response status
int tx_statusLed = 13;
int tx_errorLed = 13;

//set RX indicator LED at Pin 13 to show RX status and RX response status
int rx_statusLed = 12;
int rx_errorLed = 12;

void setup()
{
	pinMode(tx_statusLed, OUTPUT);
	pinMode(tx_errorLed, OUTPUT);
	pinMode(rx_statusLed, OUTPUT);
	pinMode(rx_errorLed, OUTPUT);
	Serial.begin(9600);
	Serial1.begin(9600);
	//set Serial1 to xbee module
	master_xbee.setSerial(Serial1);

}

// the loop function runs over and over again until power down or reset
void loop()
{
	inquireTxCom();
	inquireRxCom();

}
//the Led_flash function
void flashLed(int pin, int times, int wait)
{

	for (int i = 0; i < times; i++)
	{
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);

		if (i + 1 < times)
		{
			delay(wait);
		}
	}
}
//Send inquiry order and check order sending status
void inquireTxCom()
{
	//send the TX request message
	master_xbee.send(zbTx);
	// flash TX indicator LED to show having already send the TX request message
	flashLed(tx_statusLed, 1, 100);

	// after sending a tx request, we expect a status response
	// wait up to half second for the status response
	if (master_xbee.readPacket(500))
	{
		// got a response!

		// should be a znet tx status            	
		if (master_xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
		{
			master_xbee.getResponse().getZBTxStatusResponse(txStatus);

			// get the delivery status, the fifth byte
			if (txStatus.getDeliveryStatus() == SUCCESS)
			{
				// success!!---  time to celebrate
				flashLed(tx_statusLed, 5, 50);
			}
			else
			{
				// failure！！！---the remote XBee did not receive our packet. is it powered on?
				flashLed(tx_errorLed, 3, 500);
			}
		}
	}
	else if (master_xbee.getResponse().isError())
	{
		Serial.print("Error reading packet.  Error code: ");
		Serial.println(master_xbee.getResponse().getErrorCode());
	}
	else
	{
		// local XBee did not provide a timely TX Status Response -- should not happen
		flashLed(tx_errorLed, 2, 50);
	}

	delay(1000);
}
//receive inquire resuit or receive ModemStatusResponse  
void inquireRxCom()
{
	//receive RX packet
	master_xbee.readPacket();

	if (master_xbee.getResponse().isAvailable()) {
		// got something

		if (master_xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
			// got a zb rx packet

			// now fill our zb rx class
			master_xbee.getResponse().getZBRxResponse(rx);

			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
				// the sender got an ACK
				flashLed(rx_statusLed, 10, 10);
			}
			else {
				// we got it (obviously) but sender didn't get an ACK
				flashLed(rx_errorLed, 2, 20);
			}
			// print the result of inquiring
			inquireResultPinter();
		}
		else if (master_xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
			master_xbee.getResponse().getModemStatusResponse(msr);
			// the local XBee sends this response on certain events, like association/dissociation

			if (msr.getStatus() == ASSOCIATED) {
				// this is great.  flash led
				flashLed(rx_statusLed, 10, 10);
			}
			else if (msr.getStatus() == DISASSOCIATED) {
				// this is awful.. flash led to show our discontent
				flashLed(rx_errorLed, 10, 10);
			}
			else {
				// another status
				flashLed(rx_statusLed, 5, 10);
			}
		}
		else {
			// not something we were expecting
			flashLed(rx_errorLed, 1, 25);
		}
	}
	else if (master_xbee.getResponse().isError()) {
		Serial.print("Error reading packet.  Error code: ");
		Serial.println(master_xbee.getResponse().getErrorCode());
	}
}
void inquireResultPinter()
{
	uint8_t* _data;
	_data = rx.getData();
	for (size_t i = 0; i < rx.getDataLength(); i++)
	{
		Serial.write(_data[i]);
	}

}