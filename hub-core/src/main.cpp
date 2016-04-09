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
#include "sqlite3.h"
#include "DBHandler.h"
#include "NewState.h"
#include <fstream>
#include "RuleTimer.h"
#include "RuleBuffer.h"
int main(int argc, char* argv[])
{

	std::ifstream file("database.sql");
	std::string str;
	std::string sqlString;
	while (std::getline(file, str))
	{
		sqlString += str;
		sqlString += " ";
	}

	DBHandler db;
	db.sqlExec(sqlString);
	db.closeDB();

	MQTTMessageBuffer inBuffer(10);
	MQTTMessageBuffer outBuffer(10);
	RuleBuffer ruleBuffer(10);

	MQTTHandler mqttInstance{&inBuffer,&outBuffer};
	std::thread PublishThread(&MQTTHandler::publishOutBuffer,&mqttInstance);

	RuleTimer time(10,&ruleBuffer,&outBuffer);
	std::thread TimerFireThread(&RuleTimer::startTimer,&time);
	std::thread RuleBufferResolutionThread(&RuleTimer::ruleBufferResolution,&time);

	std::cout << "Starting reflector\n";
	while (true)
	{
		//TODO Check if setup
		MQTTMessage message = inBuffer.remove();
		NewState state(message,&outBuffer);
		//if(message.getTopic() == "/esp1/switch1")
		//{
		//	MQTTMessage message2{"/esp1/led1",message.getMessage()};
		//	outBuffer.add(message2);
		//}

	}

}
