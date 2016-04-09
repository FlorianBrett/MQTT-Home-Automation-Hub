/*
 * Rule.h
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */

#ifndef RULE_H_
#define RULE_H_
#include "Constraint.h"
#include "Action.h"
#include <vector>
#include <string>
class Rule {
private:
	std::vector<Constraint> constraints;
	std::vector<Action> actions;
	std::string ruleID;
public:
	Rule(std::string inRuleID);
	void loadRule();
	bool resolveRule();
	void commitActions(MQTTMessageBuffer *outBufferPointer);
	virtual ~Rule();
};

#endif /* RULE_H_ */
