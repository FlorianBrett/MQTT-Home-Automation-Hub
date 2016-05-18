/*
 * DBHandlerMYSQL.cpp
 *
 *  Created on: 25 Apr 2016
 *      Author: flo
 */

#include "DBHandlerMYSQL.h"
#include<iostream>
#include<mysql.h>
#include<stdio.h>
#include "spdlog/spdlog.h"
#include <vector>

#define address "127.0.0.1"
#define user "127.0.0.1"
#define password "127.0.0.12"
#define database "127.0.0.1"

DBHandlerMYSQL::DBHandlerMYSQL() {
	logger = spdlog::get("DB");

	connection = mysql_init(NULL);
	if (!connection)
	{
		logger->critical() << "MySQL Initialization failed";
	}

	connection = mysql_real_connect(connection, address , "root", "brettnet" , "ruleEngine" ,0,NULL,0);
	if (connection)
	{
		logger->debug() << "Mysql connection Succeeded";
	}
	else
	{
		logger->critical() << "MySQL connection failed";
	}

}
std::string DBHandlerMYSQL::loadConfig(std::string configName)
{
	std::string sql;
	std::string returnResult;
	sql = "SELECT value FROM config WHERE name = '" + configName + "';";
	logger->trace() << "loadConfig SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	row = mysql_fetch_row(result);
	if(row != NULL){
		returnResult = row[0];
		logger->debug() <<"Config loaded " << configName << " : " << returnResult;
	}
	else
		logger->error() << "Config could not be loaded for " << configName;

	mysql_free_result(result);

	return returnResult;
}

std::string DBHandlerMYSQL::getStateValue(std::string deviceID,std::string fieldID)
{
	std::string sql;
	std::string returnResult;
	sql = "SELECT field_value FROM field WHERE device_id = '" + deviceID + "' AND field_id = '" + fieldID + "';";
	logger->trace() << "getStateValue SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	row = mysql_fetch_row(result);
	if(row != NULL) {
		if(row[0] != NULL)
			returnResult = row[0];
		else
			returnResult = "EMPY";

		logger->debug() << "State loaded for " << deviceID << "/" << fieldID << " : " << returnResult;

	}
	else
		logger->error() << "State could not be loaded for " << deviceID << "/" << fieldID;

	mysql_free_result(result);

	return returnResult;
}

void DBHandlerMYSQL::setStateValue(std::string deviceID,std::string fieldID,std::string value)
{
	std::string sql;
	sql = "UPDATE field SET field_value = " + value + " WHERE device_id = '" + deviceID + "' AND field_id = '" + fieldID + "';";
	logger->trace() << "setStateValue SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	logger->debug() << "State set for " << deviceID << "/" << fieldID << " to: " << value;

	if(loadConfig("state_history").compare("1") == 0)
		addStateHistory(deviceID, fieldID, value);
}
void DBHandlerMYSQL::addStateHistory(std::string deviceID,std::string fieldID,std::string value)
{
	std::string sql;
	sql = "INSERT INTO state_history(device_id,field_id,state_time,state_value) VALUES ('" + deviceID + "','" + fieldID + "',NOW(),'" + value + "');";
	logger->trace() << "addStateHistory SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	logger->debug() << "State History inserted for " << deviceID << "/" << fieldID <<" : " << value;

}

void DBHandlerMYSQL::addRuleHistory(std::string ruleID)
{
	std::string sql;
	sql = "INSERT INTO rule_history(rule_id,rule_history_time) VALUES (" + ruleID + ",NOW());";
	logger->trace() << "addRuleHistorySQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	logger->debug() << "Rule History inserted for " << ruleID;

}

std::vector<std::string> DBHandlerMYSQL::getRuleIDs(std::string deviceID,std::string fieldID)
{
	std::string sql;
	std::vector<std::string> results;
	//sql = "SELECT DISTINCT rule_id FROM rule_constraint WHERE field_id ='" + fieldID + "';";
	sql = "SELECT DISTINCT rule_constraint.rule_id FROM rule_constraint INNER JOIN rule ON rule.rule_id=rule_constraint.rule_id WHERE rule.rule_active=1 AND rule_constraint.field_id ='" + fieldID + "';";
	logger->trace() << "getRuleIDs SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);
	std::string debug = "getRuleIDs Results: ";
	while ((row = mysql_fetch_row(result)) !=NULL )
	{
		results.push_back(row[0]);
		debug += std::string(row[0]) + " ";
	}
	logger->debug(debug);
	mysql_free_result(result);

	return results;
}

std::vector<std::string> DBHandlerMYSQL::getConstraintIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sql = "SELECT constraint_id FROM rule_constraint WHERE rule_id=" + ruleID + ";";
	logger->trace() << "getConstraintIDs SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	std::string debug = "getConstraintIDs Results: ";
	while ((row = mysql_fetch_row(result)) !=NULL )
	{
		results.push_back(row[0]);
		debug += std::string(row[0]) + " ";
	}
	logger->debug(debug);

	mysql_free_result(result);

	return results;
}

std::vector<std::string> DBHandlerMYSQL::getConstraint(std::string constraintID)
{
	std::string sql;
	std::vector<std::string> results;
	sql = "SELECT field.device_id, rule_constraint.field_id, rule_constraint.constraint_operator, rule_constraint.constraint_value FROM rule_constraint INNER JOIN field ON field.field_id=rule_constraint.field_id WHERE constraint_id=" + constraintID + ";";
	logger->trace() << "getConstraint SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	row = mysql_fetch_row(result);
	if(row != NULL) {
		results.push_back(row[0]);
		results.push_back(row[1]);
		results.push_back(row[2]);
		results.push_back(row[3]);
		logger->debug() << "getConstraint Results: " << row[0] << " " << row[1] << " " << row[2] << " " << row[3];

	}
	else
		logger->error() << "Constraint could not be loaded for constraintID: " << constraintID;

	mysql_free_result(result);

	return results;
}

std::vector<std::string> DBHandlerMYSQL::getActionIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sql = "SELECT action_id FROM rule_action WHERE rule_id=" + ruleID + ";";
	logger->trace() << "SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	std::string debug = "getActionIDs Results: ";
	while ((row = mysql_fetch_row(result)) !=NULL )
	{
		results.push_back(row[0]);
		debug += std::string(row[0]) + " ";
	}
	logger->debug(debug);

	mysql_free_result(result);

	return results;
}

std::vector<std::string> DBHandlerMYSQL::getAction(std::string actionID)
{
	std::string sql;
	std::vector<std::string> results;
	sql = "SELECT field.device_id, rule_action.field_id, rule_action.action_operator, rule_action.action_value FROM rule_action INNER JOIN field ON field.field_id=rule_action.field_id WHERE action_id=" + actionID + ";";
	logger->trace() << "getAction SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	row = mysql_fetch_row(result);
	if(row != NULL) {
		results.push_back(row[0]);
		results.push_back(row[1]);
		results.push_back(row[2]);
		results.push_back(row[3]);
		logger->debug() << "getConstraint Results: " << row[0] << " " << row[1] << " " << row[2] << " " << row[3] << " : ";

	}
	else
		logger->error() << "Action could not be loaded for actionID: " << actionID;

	mysql_free_result(result);

	return results;
}

std::vector<std::array<int,2>> DBHandlerMYSQL::getTimerRules(int startTime, int finishTime)
{

	std::string sql;
	std::vector<std::array<int,2>> results;
	sql = "SELECT rule_id, constraint_value FROM rule_constraint WHERE (constraint_operator = 'time>' OR constraint_operator = 'time<') AND constraint_value >= " + std::to_string(startTime) + " AND constraint_value <= " + std::to_string(finishTime) + " ORDER BY constraint_value DESC;";
	logger->trace() << "getTimerRules SQL Statement : " << sql;

	mysql_query(connection,sql.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(connection);

	std::string debug = "getTimerRules Results: ";
	while ((row = mysql_fetch_row(result)) !=NULL )
	{
		char* ruleID = row[0];
		char* constraintValue = row[1];
		results.push_back(std::array<int,2>{atoi(ruleID),atoi(constraintValue)});

		debug += '[' + atoi(ruleID) + ',' + atoi(constraintValue) + ']';
	}
	logger->debug(debug);

	return results;
}

void DBHandlerMYSQL::closeDB()
{
	mysql_close(connection);
	logger->debug() << "MySQL connection closed";
}

DBHandlerMYSQL::~DBHandlerMYSQL() {
	//mysql_close(connection);
}

