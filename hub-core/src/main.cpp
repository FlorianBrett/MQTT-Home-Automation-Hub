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
	MQTTMessageBuffer buffer;

	MQTTMessage a{"topica","messagea"};
	MQTTMessage b{"topicb","messageb"};
	MQTTMessage c{"topicc","messagec"};

	buffer.add(a);
	buffer.add(b);
	buffer.add(c);
	//MQTTMessage m1,m2,m3;
	MQTTMessage m1 = buffer.remove();
	MQTTMessage m2 = buffer.remove();
	MQTTMessage m3 = buffer.remove();


	std::cout << "Buffer[1] Topic: " << m1.getTopic() << "Message:"<< m1.getMessage()<< "\n";
	std::cout << "Buffer[2] Topic: " << m2.getTopic() << "Message:"<< m2.getMessage()<< "\n";
	std::cout << "Buffer[3] Topic: " << m3.getTopic() << "Message:"<< m3.getMessage()<< "\n";

	MQTTHandler test;
	//test.getConnectionStatus();
	while (true)
	{

		usleep(100000L);
		//std::cout << "connected:" << test.getConnectionStatus()<<"\n";
	}

	//std::cout << "connected:" << test.getConnectionStatus()<<"\n";

}
