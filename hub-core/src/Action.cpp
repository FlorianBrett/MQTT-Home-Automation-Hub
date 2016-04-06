/*
 * Action.cpp
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */

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
	fieldID = action[0];
	deviceID = action[1];
	actionOperator = action[2];
	actionValue = action[3];
	std::cout <<"Action Loaded: " << fieldID << " " << actionOperator << " "<< actionValue << " "  <<"\n";
}
void Action::commitAction(MQTTMessageBuffer *outBufferPointer)
{
	std::string topic = "/" + deviceID + "/" + fieldID;
	MQTTMessage outMessage{topic,actionValue};
	outBufferPointer->add(outMessage);
	std::cout <<"Action Commited: " << topic << " " << actionOperator << " "<< actionValue << " "  <<"\n";
}

Action::~Action() {
	// TODO Auto-generated destructor stub
}

