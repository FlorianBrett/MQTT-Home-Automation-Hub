/*
 * Constraint.cpp
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */
#include "DBHandler.h"
#include "Constraint.h"
#include <string>
#include <iostream>
Constraint::Constraint(std::string constraintID) {
	DBHandler db;
	std::vector<std::string> constraint = db.getConstraint(constraintID);
	db.closeDB();
	deviceID = constraint[0];
	fieldID = constraint[1];
	constraintOperator = constraint[2];
	constraintValue = constraint[3];
}
bool Constraint::resolveConstraint() {

	bool resolution = false;
	DBHandler db;
	std::string fieldValue = db.getStateValue2(deviceID,fieldID);
	db.closeDB();
	if (constraintOperator.compare("=") == 0) {
		if (atoi(fieldValue.c_str()) == atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare("<") == 0) {
		if (atoi(fieldValue.c_str()) < atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare(">") == 0) {
		if (atoi(fieldValue.c_str()) > atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare("<=") == 0) {
		if (atoi(fieldValue.c_str()) <= atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare(">=") == 0) {
		if (atoi(fieldValue.c_str()) >= atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare("!=") == 0) {
		if (atoi(fieldValue.c_str()) != atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare("time>=") == 0)
	{
		time_t currentTime;
		struct tm *timeData;
		time (&currentTime);
		timeData = localtime (&currentTime);
		int secondsOfDay = (timeData->tm_hour * (60 * 60)) + (timeData->tm_min * 60) + timeData->tm_sec;

		if (secondsOfDay >= atoi(constraintValue.c_str()))
			resolution = true;
	}
	else if (constraintOperator.compare("time<=") == 0)
	{
		time_t currentTime;
		struct tm *timeData;
		time (&currentTime);
		timeData = localtime (&currentTime);
		int secondsOfDay = (timeData->tm_hour * (60 * 60)) + (timeData->tm_min * 60) + timeData->tm_sec;

		if (secondsOfDay <= atoi(constraintValue.c_str()))
			resolution = true;
	}
	return resolution;
}
Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

