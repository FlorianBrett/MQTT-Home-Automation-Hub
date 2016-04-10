/*
 * DBHandler.h
 *
 *  Created on: 12 Mar 2016
 *      Author: flo
 */

#ifndef DBHANDLER_H_
#define DBHANDLER_H_

#include "sqlite3.h"
#include <string>
#include <queue>
#include <array>
#include "spdlog/spdlog.h"

class DBHandler {
private:
	static sqlite3 *db;
	static int cb_Output(void *NotUsed, int argc, char **argv, char **azColName);
	std::shared_ptr<spdlog::logger> logger;
public:
	DBHandler();
	void closeDB();
	void sqlExec(char* inSQL);
	void sqlExec(std::string sql);
	char* selectConfig(std::string name);
	std::string loadConfig(std::string name);
	std::string getStateValue(std::string device,std::string field);
	void setStateValue(std::string device,std::string field,std::string value);
	void addStateHistory(std::string device,std::string field,std::string value);
	std::vector<std::string> getRuleIDs(std::string device,std::string field);
	std::vector<std::string> getConstraintIDs(std::string ruleID);
	std::vector<std::string> getConstraint(std::string constraintID);
	std::vector<std::string> getActionIDs(std::string ruleID);
	std::vector<std::string> getAction(std::string actiontID);
	std::vector<std::array<int,2>> getTimerRules();
	std::vector<std::array<int,2>> getTimerRules(int startTime, int finishTime);
	virtual ~DBHandler();
};

#endif /* DBHANDLER_H_ */
