/*
 * Action.cpp
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */
#include "DBHandler.h"
#include "Action.h"
#include "MQTTMessageBuffer.h"
#include <string>
#include <iostream>
//#include "MQTTMessage.h"
Action::Action(std::string actionID) {
	DBHandler db;
	std::vector<std::string> action = db.getAction(actionID);
	//db.~DBHandler();
	db.closeDB();
	deviceID = action[0];
	fieldID = action[1];
	actionOperator = action[2];
	actionValue = action[3];
	std::cout <<"Action Loaded: " << fieldID << " " << actionOperator << " "<< actionValue << " "  <<"\n";
}
void Action::commitAction(MQTTMessageBuffer *outBufferPointer)
{
	DBHandler db;
	std::string fieldValue = db.getStateValue2(deviceID,fieldID);
	db.closeDB();
	std::string topic = "/" + deviceID + "/" + fieldID;
	std::string message;
	if (actionOperator.compare("=") == 0)
	{
		message = actionValue;
		std::cout <<"Action(=): " << message <<"\n
	}
	else if (actionOperator.compare("+") == 0)
	{
		int newValue = atoi(fieldValue.c_str()) + atoi(actionValue.c_str());
		message = std::to_string(newValue);
		std::cout <<"Action(+): " << fieldValue << " " << actionOperator << " " << actionValue << " = " << message <<"\n";
	}
	else if (actionOperator.compare("-") == 0)
	{
		int newValue = atoi(fieldValue.c_str()) - atoi(actionValue.c_str());
		message = std::to_string(newValue);
		std::cout <<"Action(-): " << fieldValue << " " << actionOperator << " " << actionValue << " = " << message <<"\n";
	}
	else if (actionOperator.compare("toggle") == 0)
	{
		std::cout <<"Action(toggle): ";
		if (atoi(fieldValue.c_str()) == 1)
		{
			message = "0";
		}
		else
		{
			message = "1";
		}
		std::cout <<"Toggle from: " << fieldValue << " to " << message << "\n";
	}
	MQTTMessage outMessage{topic,message};
	outBufferPointer->add(outMessage);
	std::cout <<"Action Commited: " << deviceID << "/" << fieldID << ": " << actionOperator << " "<< actionValue << " "  <<"\n";
}

Action::~Action() {
	// TODO Auto-generated destructor stub
}

