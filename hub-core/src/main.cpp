/*
 * main.cpp
 *
 *  Home Automation Hub Core
 *      Author: Florian Brett
 */
#include <string>
#include <iostream>
#include <thread>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTHandler.h"
#include <unistd.h>
#include "MQTTMessageBuffer.h"
#include "MQTTMessage.h"
int main(int argc, char* argv[])
{
	MQTTMessageBuffer inBuffer;
	MQTTMessageBuffer outBuffer;

	MQTTHandler mqttInstance{&inBuffer,&outBuffer};
	std::thread PublishThread(&MQTTHandler::publishOutBuffer,&mqttInstance);

	std::cout << "Starting reflector\n";
	while (true)
	{

		MQTTMessage message = inBuffer.remove();
		MQTTMessage message2{"/esp1/led1",message.getMessage()};
		outBuffer.add(message2);
		//usleep(100000L);
		//std::cout << "connected:" << test.getConnectionStatus()<<"\n";
	}

	//std::cout << "connected:" << test.getConnectionStatus()<<"\n";

}
