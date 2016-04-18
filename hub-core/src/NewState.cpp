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
#include "spdlog/spdlog.h"

bool NewState::checkStateChange() {
	bool stateChanged = false;
	DBHandler db;
	std::string originalValue = db.getStateValue(device,field);
	logger->debug() <<"Comparing: '" << value << "' AND '" << originalValue << "'";
	if (value.compare(originalValue) != 0)
	{
		logger->info() << "State Changed.";
		stateChanged = true;
	}
	else
		logger->info() << "State not Changed.";
	db.closeDB();
	return stateChanged;
}
NewState::NewState(MQTTMessage inMessage,MQTTMessageBuffer *outBufferPointer) {
	logger = spdlog::get("GLOBAL");
	std::string topic = inMessage.getTopic();
	topic.erase(0,1);
	int pos = topic.find("/");
	device = topic.substr(0, pos);
	topic.erase(0, pos + 1);
	field = topic;
	value = inMessage.getMessage();
	logger->info() << "New State: " << device << "/" << field << "=" << value;

	if (checkStateChange() == true)
	{
		DBHandler db;
		db.setStateValue(device,field,value);
		std::vector<std::string> ruleIDs = db.getRuleIDs(device,field);

		db.closeDB();
		if(ruleIDs.size() > 0)
		{
			std::string debug = "Rules found for " + device + "/" + field + ": ";
			for(std::vector<std::string>::iterator it = ruleIDs.begin(); it != ruleIDs.end(); ++it)
			{
				debug += (*it + " ");
				Rule tempRule(*it);

				rules.push_back(tempRule);
			}
			logger->info(debug);
			for(std::vector<Rule>::iterator it = rules.begin(); it != rules.end(); ++it)
			{
				it->loadRule();
				if(it->resolveRule() == true)
					it->commitActions(outBufferPointer);
			}
		}
		else
		{
			logger->info() << "No rules found for "<< device << "/" << field;
		}
	}
	logger->info() << "New State complete";
}

NewState::~NewState() {
	// TODO Auto-generated destructor stub
}

