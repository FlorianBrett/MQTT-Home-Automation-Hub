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

int main(int argc, char* argv[])
{
	std::cout << "hello world";
	MQTTHandler test;
	//test.getConnectionStatus();
	while (true)
	{

		usleep(100000L);
		std::cout << "connected:" << test.getConnectionStatus()<<"\n";
		//std::cin.ignore();
	}

	//std::cout << "connected:" << test.getConnectionStatus()<<"\n";

}
