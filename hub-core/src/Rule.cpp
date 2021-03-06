/*
 * Rule.cpp
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */

#include "Rule.h"
#include "Constraint.h"
#include "Action.h"
#include "DBHandler.h"
#include <string>
#include <vector>
#include <iostream>
#include "spdlog/spdlog.h"
#include "DBHandlerMYSQL.h"
Rule::Rule(std::string inRuleID) {
	ruleID=inRuleID;
}
void Rule::loadRule() {
	DBHandlerMYSQL db;
	std::vector<std::string> constraintIDs = db.getConstraintIDs(ruleID);
	std::vector<std::string> actionIDs = db.getActionIDs(ruleID);
	db.closeDB();
	for(std::vector<std::string>::iterator it = constraintIDs.begin(); it != constraintIDs.end(); ++it) {
		Constraint tempConstraint(*it);
		constraints.push_back(tempConstraint);
	}
	for(std::vector<std::string>::iterator it = actionIDs.begin(); it != actionIDs.end(); ++it) {
		Action tempAction(*it);
		actions.push_back(tempAction);
	}
}
bool Rule::resolveRule() {
	auto logger = spdlog::get("RULE");
	bool resolution = true;
	for(std::vector<Constraint>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
		if(it->resolveConstraint() == false)
		{
			resolution = false;
			break;
		}
	}
	if (resolution == true) {
		DBHandlerMYSQL db;
		logger->info() << "Rule" << ruleID  << ": resolved TRUE";
		if(db.loadConfig("rule_history").compare("1") == 0)
			db.addRuleHistory(ruleID);
		db.closeDB();
	}
	else
		logger->info() << "Rule" << ruleID  << ": resolved FALSE";
	return resolution;
}
void Rule::commitActions(MQTTMessageBuffer *outBufferPointer) {

	for(std::vector<Action>::iterator it = actions.begin(); it != actions.end(); ++it) {
		it->commitAction(outBufferPointer);
	}
}
Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

