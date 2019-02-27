/*
 Name:		LoraClient.ino
 Created:	1/24/2018 4:25:18 PM
 Author:	jtext103

get temp data and sent to the Lora Server
This is an example to show how to get sensor data from a remote Arduino via Wireless Lora Protocol£¬unaddressd concept
*/


#include <RH_RF95.h>
#include <SPI.h>
/*#include <MAX31855_ThermoSensor.h>
#include <PWM_PinSensor.h>
#include <SensorBase.h>
#include <Host.h>
#include<CommunicationBase.h>
#include <ModbusRTUComm.h>
#include <Timer.h>
#include <DigitalPin.h>
*/

/*create a RH_RF95 object by using defaults parameters(ss:D10 for Diecimila, Uno etc, D53 for Mega, D10 for Maple;
 interruptPin = 2;spi = hardware_spi )
*/
RH_RF95 rf95;

float data;
String datastring = "";
char databuf[10];
uint8_t dataoutgoing[10];
// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(9600);
	if (!rf95.init())
		Serial.println("init failed");
	// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

// the loop function runs over and over again until power down or reset
void loop()
{
	// Print Sending to rf95_server
	Serial.println("Sending to rf95_server");

	
	data = 10010;
	datastring += dtostrf(data, 4, 2, databuf);
	strcpy((char *)dataoutgoing, databuf);
	Serial.println(databuf);
	rf95.send(dataoutgoing, sizeof(dataoutgoing));
	rf95.waitPacketSent();
	// Now wait for a reply
	uint8_t indatabuf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(indatabuf);

	if (rf95.waitAvailableTimeout(3000))
	{
		// Should be a reply message for us now   
		if (rf95.recv(indatabuf, &len))
		{
			// Serial print "got reply:" and the reply message from the server
			Serial.print("got reply: ");
			Serial.println((char*)indatabuf);
		}
		else
		{
			//
			Serial.println("recv failed");
		}
	}
	else
	{
		// Serial print "No reply, is rf95_server running?" if don't get the reply .
		Serial.println("No reply, is rf95_server running?");
	}
	delay(400);
}
