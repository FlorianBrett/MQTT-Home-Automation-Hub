/*
 * DBHandler.cpp
 *
 *  Created on: 12 Mar 2016
 *      Author: flo
 */

#include "DBHandler.h"
#include "sqlite3.h"
#include "stdio.h"
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include "spdlog/spdlog.h"

sqlite3 *DBHandler::db = NULL;
int DBHandler::cb_Output(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   data = (void *)azColName[1];
   for(i=0; i<argc; i++){

      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
void DBHandler::sqlExec(char* inSQL)
{
	logger->trace() << "SQL Statement : " << inSQL;

	char *ErrMsg = 0;
	int  rc;
	rc = sqlite3_exec(db, inSQL, &cb_Output, 0, &ErrMsg);
	if( rc != SQLITE_OK ){
		logger->error() << "SQL error: " << ErrMsg;
		sqlite3_free(ErrMsg);
	}
	else{
		logger->debug() << "SQL Executed successfully" << ErrMsg;
	}
}
void DBHandler::sqlExec(std::string sql)
{
	logger->trace() << "SQL Statement : " << sql;
	char *ErrMsg = 0;
	int  rc;
	rc = sqlite3_exec(db, sql.c_str(), &cb_Output, 0, &ErrMsg);
	if( rc != SQLITE_OK ){
		logger->error() << "SQL error: " << ErrMsg;
		sqlite3_free(ErrMsg);
	}
	else{
		logger->debug() << "SQL Executed successfully" << ErrMsg;
	}
}
char* DBHandler::selectConfig(std::string name)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT * FROM CONFIG WHERE name = '" + name + "';";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			result = (char*)sqlite3_column_text(sqlStatement, 2);
			logger->debug() <<"Config loaded " << name << " : " << std::string(result);
		}
	}
	else
		logger->error() << "Sqlite prepare fail";

	return result;
}
std::string DBHandler::loadConfig(std::string configName)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT value FROM config WHERE name = '" + configName + "';";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			result = (char*)sqlite3_column_text(sqlStatement, 0);
			logger->debug() <<"Config loaded " << configName << " : " << std::string(result);
		}
	}
	else
		logger->error() << "Sqlite prepare fail";

	if (result != NULL)
		logger->debug() <<" Config loaded for " << configName << " : " << std::string(result);
	else
	{
		logger->debug() << "Config could not be loaded for " << configName;
		result = "NOTFOUND";
	}
	std::string returnResult = std::string(result);

	return returnResult;
}
std::string DBHandler::getStateValue(std::string device,std::string field)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	std::string returnResult;

	sql = "SELECT field_value FROM field WHERE device_id = '" + device + "' AND field_id = '" + field + "';";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			if ((char*)sqlite3_column_text(sqlStatement, 0) != NULL)
			{
				returnResult = std::string((char*)sqlite3_column_text(sqlStatement, 0));
				logger->debug() <<" state loaded for " << field << " : " <<returnResult ;
			}
			else {
				logger->debug() <<" state empty state loaded for " << field;
				returnResult = "EMPTY";
			}
		}
		else {
			logger->debug() <<"state does not exist" << field;
			returnResult = "NOTFOUND";
		}

	}
	else
		logger->error() << "Sqlite prepare fail";
	sqlite3_finalize(sqlStatement);

	return returnResult;
}
void DBHandler::setStateValue(std::string device,std::string field,std::string value)
{
	std::string sql;
	sqlite3_stmt *sqlStatement;
	sql = "UPDATE field SET field_value = " + value + " WHERE device_id = '" + device + "' AND field_id = '" + field + "';";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK) {
		std::cout << "set result code: "<< field << " / " << sqlite3_step(sqlStatement) <<  "\n";
		logger->debug() << "State updated for " << device << "/" << field <<" to: " << value;
	}
	else
		logger->error() <<"sqlite Prepare fail" << '\n';

	sqlite3_finalize(sqlStatement);

	if(loadConfig("state_history").compare("1") == 0)
		addStateHistory(device, field, value);
}
void DBHandler::addStateHistory(std::string device,std::string field,std::string value)
{
	std::string sql;
	sqlite3_stmt *sqlStatement;
	//TODO Ensure time stored is correct(Could be an hour out)
	sql = "INSERT INTO state_history(device_id,field_id,state_time,state_value) VALUES ('" + device + "','" + field + "',CURRENT_TIMESTAMP,'" + value + "');";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK) {
		sqlite3_step(sqlStatement);
		logger->debug() << "State History inserted for " << device << "/" << field <<" : " << value;
	}
	else
		logger->error() <<"sqlite Prepare fail";
	sqlite3_finalize(sqlStatement);

}
void DBHandler::addRuleHistory(std::string ruleID)
{
	std::string sql;
	sqlite3_stmt *sqlStatement;
	//TODO Ensure time stored is correct(Could be an hour out)
	sql = "INSERT INTO rule_history(rule_id,rule_history_time) VALUES (" + ruleID + ",CURRENT_TIMESTAMP);";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK) {
		sqlite3_step(sqlStatement);
		logger->debug() << "Rule History inserted for " << ruleID;
	}
	else
		logger->error() <<"sqlite Prepare fail";
	sqlite3_finalize(sqlStatement);

}
std::vector<std::string> DBHandler::getRuleIDs(std::string device,std::string field)
{
	//TODO Get only active rules
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	//sql = "SELECT DISTINCT rule_id FROM rule_constraint WHERE field_id ='" + field + "';";
	sql = "SELECT rule_constraint.rule_id, FROM rule_constraint INNER JOIN rule ON rule_constraint.rule_id=rule.rule_id WHERE rule.rule_active=1 AND field_id ='" + field + "';";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::string debug = "getRuleIDs Results: ";
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 0)) + ",";
		}
		logger->debug(debug);
	}
	else
		logger->error() <<"sqlite Prepare fail" << '\n';
	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getConstraintIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT constraint_id FROM rule_constraint WHERE rule_id=" + ruleID + ";";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::string debug = "getConstraintIDs Results: ";
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 0)) + ",";
		}
		logger->debug(debug);
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getConstraint(std::string constraintID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT field.device_id, rule_constraint.field_id, rule_constraint.constraint_operator, rule_constraint.constraint_value FROM rule_constraint INNER JOIN field ON field.field_id=rule_constraint.field_id WHERE constraint_id=" + constraintID + ";";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 1)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 2)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 3)));

			std::string debug = "getConstraint Results: ";
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 0)) + ',';
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 1)) + ',';
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 2)) + ',';
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 3)) + ',';
			logger->debug(debug);
		}
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getActionIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT action_id FROM rule_action WHERE rule_id=" + ruleID + ";";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::string debug = "getActionIDs Results: ";
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			debug += std::string((char*)sqlite3_column_text(sqlStatement, 0)) + ",";
		}
		logger->debug(debug);
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getAction(std::string actionID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT field.device_id, rule_action.field_id, rule_action.action_operator, rule_action.action_value FROM rule_action INNER JOIN field ON field.field_id=rule_action.field_id WHERE action_id=" + actionID + ";";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 1)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 2)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 3)));

			std::string debug = "getAction Results: ";
			debug += (char*)sqlite3_column_text(sqlStatement, 0) + '.';
			debug += (char*)sqlite3_column_text(sqlStatement, 1) + '.';
			debug += (char*)sqlite3_column_text(sqlStatement, 2) + '.';
			debug += (char*)sqlite3_column_text(sqlStatement, 3) + '.';
			logger->debug(debug);
		}
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::array<int,2>> DBHandler::getTimerRules()
{
	std::string sql;
	std::vector<std::array<int,2>> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT rule_id, constraint_value FROM rule_constraint WHERE constraint_operator = 'time>' OR constraint_operator = 'time<' ORDER BY constraint_value DESC;";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::string debug = "getTimerRules Results: ";
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			int ruleID = sqlite3_column_int(sqlStatement, 0);
			int constraintValue = sqlite3_column_int(sqlStatement, 1);
			results.push_back(std::array<int,2>{ruleID,constraintValue});
			debug += '[' + ruleID + ',' + constraintValue + ']';
		}
		logger->debug(debug);
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::array<int,2>> DBHandler::getTimerRules(int startTime, int finishTime)
{
	//TODO Get only active rules
	std::string sql;
	std::vector<std::array<int,2>> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT rule_id, constraint_value FROM rule_constraint WHERE (constraint_operator = 'time>' OR constraint_operator = 'time<') AND constraint_value >= " + std::to_string(startTime) + " AND constraint_value <= " + std::to_string(finishTime) + " ORDER BY constraint_value DESC;";
	logger->trace() << "SQL Statement : " << sql;

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::string debug = "getTimerRules Results: ";
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			int ruleID = sqlite3_column_int(sqlStatement, 0);
			int constraintValue = sqlite3_column_int(sqlStatement, 1);
			results.push_back(std::array<int,2>{ruleID,constraintValue});
			debug += '[' + ruleID + ',' + constraintValue + ']';
		}
		logger->debug(debug);
	}
	else
		logger->error() <<"sqlite Prepare fail";

	sqlite3_finalize(sqlStatement);
	return results;
}
void DBHandler::closeDB()
{
	sqlite3_close(db);
	//logger->debug() << "DB connection closed";
}


DBHandler::DBHandler() {
	logger = spdlog::get("DB");
	int  rc;
	if (db == NULL)
	{
		rc = sqlite3_open("test6.db", &db);
		if(rc){
			logger->critical() << "DB connection can not be opened: " << sqlite3_errmsg(db);
		}
		else{
			logger->info() << "DB connection opened";
		}
	}
}

DBHandler::~DBHandler() {
	//sqlite3_close(db);
	//std::cout <<"Closed DB connection " << '\n';
	// TODO Auto-generated destructor stub
}

