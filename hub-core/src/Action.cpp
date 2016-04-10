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
	db.closeDB();
	deviceID = action[0];
	fieldID = action[1];
	actionOperator = action[2];
	actionValue = action[3];
}
void Action::commitAction(MQTTMessageBuffer *outBufferPointer)
{
	DBHandler db;
	std::string fieldValue = db.getStateValue(deviceID,fieldID);
	db.closeDB();
	std::string topic = "/" + deviceID + "/" + fieldID;
	std::string message;
	if (actionOperator.compare("=") == 0)
	{
		message = actionValue;
	}
	else if (actionOperator.compare("+") == 0)
	{
		int newValue = atoi(fieldValue.c_str()) + atoi(actionValue.c_str());
		message = std::to_string(newValue);
	}
	else if (actionOperator.compare("-") == 0)
	{
		int newValue = atoi(fieldValue.c_str()) - atoi(actionValue.c_str());
		message = std::to_string(newValue);
	}
	else if (actionOperator.compare("toggle") == 0)
	{
		if (atoi(fieldValue.c_str()) == 1)
			message = "0";
		else
			message = "1";
	}
	MQTTMessage outMessage{topic,message};
	outBufferPointer->add(outMessage);
}

Action::~Action() {
	// TODO Auto-generated destructor stub
}

