#include "PWM_PinSensor.h"

PWM_PinSensor::PWM_PinSensor(uint8_t _pwmPIN,  uint8_t _dirPIN)
{
	pwmPIN = _pwmPIN;
	/*ssrPIN = _ssrPIN;*/
	dirPIN = _dirPIN;
	setPIN();
	//init
	delay(200);
	Write(0, 1);
}

PWM_PinSensor::~PWM_PinSensor()
{
}

int PWM_PinSensor::GetChnanelNum()
{
	return 1;
}

int PWM_PinSensor::Update()
{
	Handle(sample[0]);
	return 0;
}

float PWM_PinSensor::Read(int chanelNo)
{
	return sample[0];
}

int PWM_PinSensor::Write(int channelNo, float value, bool isAsync)
{
	if (value > 10)
	{
		value= 10;
	}
	if (value < -10)
	{
		value = -10;
	}

	sample[0] = value;
	/*Serial.println(" ");
	Serial.println(value);*/
	
	this->Update();
	return 0;
}

void PWM_PinSensor::Handle(float sample)
{
	identify(sample);
	analogWrite(pwmPIN, pwm_maker(sample));
}
//convert input into pwm value
int PWM_PinSensor::pwm_maker(float val)
{
	float _val;
	_val = abs(val);
	if (_val<0.2)
	{
		return 0;
	}
	
	return((int)map(_val*100, INPUT_VALUE_MIN*100, INPUT_VALUE_MAX*100, OUTPUT_VALUE_MIN, OUTPUT_VALUE_MAX));
}
//distinguish input if "+" or "-" or "0"
void PWM_PinSensor::identify(float val)
{
	/*if (abs(val) < 0.2)
	{
		digitalWrite(ssrPIN, LOW);
		return;
	}*/
	if (val >= 0.2)
	{
		/*digitalWrite(ssrPIN, HIGH);*/
		digitalWrite(dirPIN, HIGH);
		return;
	}
	if (val <= -0.2)
	{
		/*digitalWrite(ssrPIN, HIGH);*/
		digitalWrite(dirPIN, LOW);
		return;
	}
}
//set up PIN
void PWM_PinSensor::setPIN()
{
	pinMode(pwmPIN, OUTPUT);
	pinMode(dirPIN, OUTPUT);
	/*pinMode(ssrPIN, OUTPUT);*/
}
