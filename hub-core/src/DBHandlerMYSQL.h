/*
 * DBHandlerMYSQL.h
 *
 *  Created on: 25 Apr 2016
 *      Author: flo
 */

#ifndef DBHANDLERMYSQL_H_
#define DBHANDLERMYSQL_H_
#include<mysql.h>
#include<iostream>
#include "spdlog/spdlog.h"
#include <vector>

class DBHandlerMYSQL {
private:
	MYSQL *connection;
	std::shared_ptr<spdlog::logger> logger;
public:
	DBHandlerMYSQL();
	virtual ~DBHandlerMYSQL();
	void closeDB();
	std::string loadConfig(std::string configName);
	std::string getStateValue(std::string deviceID,std::string fieldID);
	void setStateValue(std::string deviceID,std::string fieldID,std::string value);
	void addStateHistory(std::string deviceID,std::string fieldID,std::string value);
	void addRuleHistory(std::string ruleID);
	std::vector<std::string> getRuleIDs(std::string deviceID,std::string fieldID);
	std::vector<std::string> getConstraintIDs(std::string ruleID);
	std::vector<std::string> getConstraint(std::string constraintID);
	std::vector<std::string> getActionIDs(std::string ruleID);
	std::vector<std::string> getAction(std::string actiontID);
	std::vector<std::array<int,2>> getTimerRules(int startTime, int finishTime);
};

#endif /* DBHANDLERMYSQL_H_ */
