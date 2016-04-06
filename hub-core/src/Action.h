/*
 * Action.h
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */

#ifndef ACTION_H_
#define ACTION_H_

#include "DBHandler.h"
#include "Constraint.h"
#include <string>
#include <iostream>
#include "MQTTMessageBuffer.h"
class Action {
private:
	std::string fieldID;
	std::string deviceID;
	std::string actionOperator;
	std::string actionValue;
public:
	Action(std::string actionID);
	void commitAction(MQTTMessageBuffer *outBufferPointer);
	virtual ~Action();
};

#endif /* ACTION_H_ */
