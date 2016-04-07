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
	//db.~DBHandler();
	db.closeDB();
	deviceID = constraint[0];
	fieldID = constraint[1];
	constraintOperator = constraint[2];
	constraintValue = constraint[3];
	std::cout <<"Constraint Loaded: " << deviceID << "/" << fieldID << " " << constraintOperator << " "<< constraintValue << " "  <<"\n";
}
bool Constraint::resolveConstraint() {
	bool resolution = false;
	DBHandler db;
	std::string fieldValue = db.getStateValue2(deviceID,fieldID);
	//db.~DBHandler();
	db.closeDB();
	if (constraintOperator.compare("=") == 0)
	{
		//if (fieldValue.compare(constraintValue) == 0)
		if (atoi(fieldValue.c_str()) == atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	else if (constraintOperator.compare("<") == 0)
	{
		if (atoi(fieldValue.c_str()) < atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(<): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(<): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	else if (constraintOperator.compare(">") == 0)
	{
		if (atoi(fieldValue.c_str()) > atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(>): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(>): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	else if (constraintOperator.compare("<=") == 0)
	{
		if (atoi(fieldValue.c_str()) <= atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(<=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(<=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	else if (constraintOperator.compare(">=") == 0)
	{
		if (atoi(fieldValue.c_str()) >= atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(>=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(>=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	else if (constraintOperator.compare("!=") == 0)
	{
		if (atoi(fieldValue.c_str()) != atoi(constraintValue.c_str()))
		{
			std::cout <<"Constraint Resolve(!=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
			resolution = true;
		}
		else
			std::cout <<"Constraint Resolve(!=): " << deviceID << "/" << fieldID << ": " << fieldValue << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";
	}
	return resolution;
}
Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

