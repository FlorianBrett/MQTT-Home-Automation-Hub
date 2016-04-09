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
		std::cout <<"State Changed. Updating DB." <<"\n";
		stateChanged = true;
	}
	else
		std::cout <<"State not Changed" <<"\n";
	//db.~DBHandler();
	db.closeDB();
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
	std::cout <<"\nNew State: " << device << "/" << field << "=" << value <<"\n";

	if (checkStateChange() == true)
	{
		DBHandler db;
		db.setStateValue2(device,field,value);
		std::vector<std::string> ruleIDs = db.getRuleIDs(device,field);
		//db.~DBHandler();
		db.closeDB();
		if(ruleIDs.size() > 0)
		{
			std::cout <<"Rules found for "<< device << "/" << field << ": ";
			for(std::vector<std::string>::iterator it = ruleIDs.begin(); it != ruleIDs.end(); ++it)
			{
				std::cout << *it << " ";
				Rule tempRule(*it);

				rules.push_back(tempRule);
			}
			std::cout << "\n";
			for(std::vector<Rule>::iterator it = rules.begin(); it != rules.end(); ++it)
			{
				it->loadRule();
				if(it->resolveRule() == true)
					it->commitActions(outBufferPointer);
			}
		}
		else
		{
			std::cout <<"No rules found for "<< device << "/" << field << "\n";
		}
	}

	// TODO Check matching rules
	// TODO Create rules
}

NewState::~NewState() {
	// TODO Auto-generated destructor stub
}

