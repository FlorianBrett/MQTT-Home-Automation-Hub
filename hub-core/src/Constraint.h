/*
 * Constraint.h
 *
 *  Created on: 16 Mar 2016
 *      Author: flo
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_
#include <string>
class Constraint {
private:
	std::string fieldID;
	std::string constraintOperator;
	std::string constraintValue;
public:
	Constraint(std::string constraintID);
	bool resolveConstraint();
	virtual ~Constraint();
};

#endif /* CONSTRAINT_H_ */
