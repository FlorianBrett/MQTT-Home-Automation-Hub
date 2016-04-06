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
	fieldID = constraint[0];
	constraintOperator = constraint[1];
	constraintValue = constraint[2];
	std::cout <<"Constraint Loaded: " << fieldID << " " << constraintOperator << " "<< constraintValue << " "  <<"\n";
}
bool Constraint::resolveConstraint() {
	bool resolution = false;
	DBHandler db;
	std::string fieldValue = db.getStateValue2("esp1",fieldID);
	//db.~DBHandler();
	db.closeDB();
	if (fieldValue.compare(constraintValue) == 0)
	{
		std::cout <<"Constraint Resolve: " << fieldID << " " << constraintOperator << " "<< constraintValue << " "  <<" is TRUE\n";
		resolution = true;
	}
	else
		std::cout <<"Constraint Resolve: " << fieldID << " " << constraintOperator << " "<< constraintValue << " "  <<" is FALSE\n";

	return resolution;
}
Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

