/*
 Name:		LoraServer.ino
 Created:	1/24/2018 9:01:35 PM
 Author:	jtext103
*/
//Include required lib so Arduino can talk with the Lora Shield

#include <RH_RF95.h>
#include <SPI.h>

RH_RF95 rf95;
int led = 13;
int reset_lora = 9;
String dataString = "";
// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(led, OUTPUT);
	pinMode(reset_lora, OUTPUT);
	Serial.begin(9600);
	// reset lora module first. to make sure it will works properly
	digitalWrite(reset_lora, LOW);
	delay(500);
	digitalWrite(reset_lora, HIGH);
	if (!rf95.init())
		Serial.println("init failed");
	// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

// the loop function runs over and over again until power down or reset
void loop()
{
	dataString = "";
	if (rf95.available())
	{
		Serial.println("Get new message");
		// Should be a message for us now   
		uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);
		if (rf95.recv(buf, &len))
		{
			digitalWrite(led, HIGH);
			//RH_RF95::printBuffer("request: ", buf, len);
			Serial.print("got message: ");
			Serial.println((char*)buf);
			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
			char responese[10] = "ok";
			rf95.send((uint8_t*)responese, sizeof(responese));
		}

	}
}