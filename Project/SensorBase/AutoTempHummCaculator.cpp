#include "AutoTempHummCaculator.h"

AutoTempHummCaculator::AutoTempHummCaculator(Host * host, int innertempNo, int innerhummNo, int outertempNo,int outerhummNo)
{
	//初始化
	temp_inner=0;
	humm_inner= 0;
	temp_outer= 0;
	humm_outer = 0;
	humm = 0;
	ab_humm = 0;
	this->host = host;
	this->innertempNo = innertempNo;
	this->innerhummNo = innerhummNo;
	this->outertempNo = outertempNo;
	this->outerhummNo = outerhummNo;
}

AutoTempHummCaculator::~AutoTempHummCaculator()
{
}

int AutoTempHummCaculator::GetChnanelNum()
{
	return 3;
}

int AutoTempHummCaculator::Update()
{
	//update the real time data from host
	humm_inner=host->ChannelSamples[innerhummNo];
	temp_inner=host->ChannelSamples[innertempNo];
	temp_outer=host->ChannelSamples[outertempNo];
	humm_outer=host->ChannelSamples[outerhummNo];
	//如果环境湿度>95%，则折算
	if (humm_outer > 95)
	{
		float a;
		float b;
		// a and b if Temp>=0°C:
		if (temp_inner > 0) {
			a = 7.5;
			b = 237.3;
		}
		// a and b if Temp<0°C:
		else {
			a = 7.6;
			b = 240.7;
		}
		// SDD1 = 折算前内部温度下饱和蒸汽压 in hPa; SDD2 = 折算后外部温度下饱和蒸汽压 in hPa; 
		//DaDr1 = 折算前内部温度下蒸汽压 in hPa;DaDr2 = 折算后外部温度下蒸汽压 in hPa;
		// TD = 露点温度. in °C; AF = 绝对湿度 in g H20 / m^3 
		float SDD1 = 6.1078 * pow(10, ((a * temp_inner) / (b + temp_inner)));
		float DaDr1 = humm_inner / 100 * SDD1;
		float v = log10(DaDr1 / 6.1078);
		float TD = b * v / (a - v);
		float AF = pow(10, 5) * 18.016 / 8314.3 * DaDr1 / (temp_inner + 273.15);//计算得到绝对湿度
		float DaDr2 = AF / pow(10, 5)*8314.3 / 18.016*(temp_outer + 273.15);
		float SDD2 = 6.1078 * pow(10, ((a * temp_outer) / (b + temp_outer)));
		humm = DaDr2 * 100 / SDD2;//将绝对湿度折算成环境温度下的相对湿度值,reference ：http://www.wetterochs.de/wetter/feuchte.html#f2
		ab_humm = AF;
	}
	//如果环境湿度<95%，则不折算
	else
	{
		float a;
		float b;
		// a and b if Temp>=0°C:
		if (temp_outer > 0) {
			a = 7.5;
			b = 237.3;
		}
		// a and b if Temp<0°C:
		else {
			a = 7.6;
			b = 240.7;
		}
		// SDD1 = 环境温度下饱和蒸汽压 in hPa 
		//DaDr1 = 环境温度下蒸汽压 in hPa
		// TD = 露点温度. in °C; AF = 绝对湿度 in g H20 / m^3 
		float SDD1 = 6.1078 * pow(10, ((a * temp_outer) / (b + temp_outer)));
		float DaDr1 = humm_outer / 100 * SDD1;
		float v = log10(DaDr1 / 6.1078);
		float TD = b * v / (a - v);
		float AF = pow(10, 5) * 18.016 / 8314.3 * DaDr1 / (temp_outer + 273.15);//计算得到绝对湿度,reference ：http://www.wetterochs.de/wetter/feuchte.html#f2
		humm = humm_outer;//湿度即为当前测量的环境湿度值
		ab_humm = AF;
	}
	return 0;
}

float AutoTempHummCaculator::Read(int chanelNo)
{
	switch (chanelNo)
	{
	case 0:return temp_outer;
		break;
	case 1:return humm;
		break;
	case 2:return ab_humm;
		break;
	default:
		break;
	}
}

int AutoTempHummCaculator::Write(int channelNo, float value, bool isAllowed)
{
	if (isAllowed == true)
	{
		switch (channelNo)
		{
		case 0:
			temp_outer = value;
			return 0;
		case 1:
			humm = value;
			return 0;
		case 2:
			ab_humm = value;
			return 0;
		default:
			break;
		}
	}
	
}
