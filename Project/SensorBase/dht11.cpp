#include"dht11.h"

//you need to give the sum channel number of dht11 and Channelname(for example,1,2...)
dht11::dht11( int pinNo)
{
	this->pinNo = pinNo;
}

dht11::~dht11()
{
}

int dht11::GetChnanelNum()
{
	//in dht11 we have two channel
	return TotalChannel;
}

int dht11::Update( )
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i = 0; i< 5; i++) bits[i] = 0;

		// REQUEST SAMPLE
		pinMode(pinNo, OUTPUT);
		digitalWrite(pinNo, LOW);
		delay(18);
		digitalWrite(pinNo, HIGH);
		delayMicroseconds(40);
		pinMode(pinNo, INPUT);


		// ACKNOWLEDGE or TIMEOUT
		unsigned int loopCnt = 10000;
		while (digitalRead(pinNo) == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		loopCnt = 10000;
		while (digitalRead(pinNo) == HIGH)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
		for (int j = 0; j < 40; j++)
		{
			loopCnt = 10000;
			while (digitalRead(pinNo) == LOW)
				if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

			unsigned long t = micros();

			loopCnt = 10000;
			while (digitalRead(pinNo) == HIGH)
				if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

			if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
			if (cnt == 0)   // next byte?
			{
				cnt = 7;    // restart at MSB
				idx++;      // next byte!
			}
			else cnt--;
		}

		// WRITE TO RIGHT VARS
		// as bits[1] and bits[3] are allways zero they are omitted in formulas.
		humidity = bits[0];
		temperature = bits[2];

		uint8_t sum = bits[0] + bits[2];

		if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
		return DHTLIB_OK;
}

float dht11::Read(int chanelNo)
{
	switch (chanelNo)
	{
	case 0:return temperature;
		break;
	case 1:return humidity;
		break;
	}
}

double dht11::Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

double dht11::Kelvin(double celsius)
{
	return celsius + 273.15;
}

double dht11::dewPoint(double celsius, double humidity)
{
	double A0 = 373.15 / (273.15 + celsius);
	double SUM = -7.90298 * (A0 - 1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1 - 1 / A0))) - 1);
	SUM += 8.1328e-3 * (pow(10, (-3.49149*(A0 - 1))) - 1);
	SUM += log10(1013.246);
	double VP = pow(10, SUM - 3) * humidity;
	double T = log(VP / 0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

double dht11::dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity / 100);
	double Td = (b * temp) / (a - temp);
	return Td;
}

/*
void dht11::Cheak()
{
	int chk = Update();

	Serial.print("Read sensor: ");
	switch (chk)
	{
	case DHTLIB_OK:
		Serial.println("OK");
		break;
	case DHTLIB_ERROR_CHECKSUM:
		Serial.println("Checksum error");
		break;
	case DHTLIB_ERROR_TIMEOUT:
		Serial.println("Time out error");
		break;
	default:
		Serial.println("Unknown error");
		break;
	}
}
*/

