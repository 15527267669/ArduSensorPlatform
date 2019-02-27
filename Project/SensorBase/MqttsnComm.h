#pragma once
#ifndef _MqttsnComm_h
 #define _MqttsnComm_h
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <CommunicationBase.h>
#include <MQTTSN_Application.h>


#include <mqttsnClientAppFw4Arduino.h>
#include <SPI.h>
#include <Ethernet/src/Ethernet.h>
#include <EthernetUdp.h>




#endif // !_MqttsnComm_h
