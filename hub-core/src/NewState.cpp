/*
 * NewState.cpp
 *
 *  Created on: 15 Mar 2016
 *      Author: flo
 */

#include "NewState.h"
#include "MQTTMessage.h"
#include "DBHandler.h"
#include <iostream>
#include <vector>
#include <iterator>
#include "Rule.h"
#include "MQTTMessageBuffer.h"

bool NewState::checkStateChange() {
	bool stateChanged = false;
	DBHandler db;
	std::string originalValue = db.getStateValue2(device,field);
	std::cout <<"Comparing: '" << value << "' AND '" << originalValue << "'" <<"\n";
	if (value.compare(originalValue) != 0)
	{
		std::cout <<"State Changed" <<"\n";
		stateChanged = true;
	}
	else
		std::cout <<"State has not Changed" <<"\n";
	//db.~DBHandler();
	db.closeDB();
	std::cout <<"seg check 1 \n";
	return stateChanged;
}
NewState::NewState(MQTTMessage inMessage,MQTTMessageBuffer *outBufferPointer) {
	std::string topic = inMessage.getTopic();
	topic.erase(0,1);
	int pos = topic.find("/");
	device = topic.substr(0, pos);
	topic.erase(0, pos + 1);
	field = topic;
	value = inMessage.getMessage();
	std::cout <<"New State: " << device << " AND " << field << " AND " << value <<"\n";

	if (checkStateChange() == true)
	{
		std::cout <<"seg check 2\n";
		DBHandler db;
		db.setStateValue2(device,field,value);
		std::vector<std::string> ruleIDs = db.getRuleIDs(device,field);
		//db.~DBHandler();
		db.closeDB();
		if(ruleIDs.size() > 0)
		{
			std::cout <<"Rules found\n";
			for(std::vector<std::string>::iterator it = ruleIDs.begin(); it != ruleIDs.end(); ++it)
			{
				Rule tempRule(*it);
				rules.push_back(tempRule);
			}

			for(std::vector<Rule>::iterator it = rules.begin(); it != rules.end(); ++it)
			{
				if(it->resolveRule() == true)
					it->commitActions(outBufferPointer);
			}
		}
		else
		{
			std::cout <<"No rules found\n";
		}
	}


	// TODO Check matching rules
	// TODO Create rules
}

NewState::~NewState() {
	// TODO Auto-generated destructor stub
}

