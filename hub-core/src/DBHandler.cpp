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
	char *zErrMsg = 0;
	int  rc;
	//inSQL = "SELECT * from COMPANY";
	//TODO 4th argument can be used for a passback
	rc = sqlite3_exec(db, inSQL, &cb_Output, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   sqlite3_free(zErrMsg);
	}else{
	   fprintf(stdout, "selected created successfully\n");
	}
}
char* DBHandler::selectConfig(std::string name)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT * FROM CONFIG WHERE name = '" + name + "';";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			result = (char*)sqlite3_column_text(sqlStatement, 2);
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';

	std::cout <<"Config loaded " << name << " : " << std::string(result) << '\n';

	return result;
}
char* DBHandler::getStateValue(std::string device,std::string field)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT * FROM field WHERE device_id = '" + device + "' AND field_id = '" + field + "';";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			result = (char*)sqlite3_column_text(sqlStatement, 5);
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	if (result == NULL)
	{
		result = "NULL";
	}
	std::cout <<"State loaded for " << device << "/" << field <<" : " << std::string(result) << '\n';
	sqlite3_finalize(sqlStatement);
	return result;
}
std::string DBHandler::getStateValue2(std::string device,std::string field)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT * FROM field WHERE device_id = '" + device + "' AND field_id = '" + field + "';";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			result = (char*)sqlite3_column_text(sqlStatement, 5);
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	if (result == NULL)
	{
		result = "NULL";
	}
	std::string returnResult = std::string(result);
	std::cout <<"State loaded for " << device << "/" << field <<" : " << returnResult << '\n';
	sqlite3_finalize(sqlStatement);
	return returnResult;
}
void DBHandler::setStateValue(std::string device,std::string field,std::string value)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "UPDATE field SET field_value='" + value + "' WHERE device_id ='" + device + "' AND field_id = '" + field + "';";
	std::cout <<"SQL Statement: " << sql << '\n';

	char *zErrMsg = 0;
	if (sqlite3_exec(db, sql.c_str(), &cb_Output, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		std::cout <<"State updated for " << device << "/" << field <<" : " << value << '\n';
	}
	sqlite3_finalize(sqlStatement);
}
void DBHandler::setStateValue2(std::string device,std::string field,std::string value)
{
	std::string sql;
	char* result;
	sqlite3_stmt *sqlStatement;
	sql = "UPDATE field SET field_value='" + value + "' WHERE device_id='" + device + "' AND field_id='" + field + "';";
	std::cout <<"SQL Statement: " << sql << '\n';

	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		std::cout <<"sqlite UPDATE COMPLETE :" << sqlite3_step(sqlStatement) <<'\n';
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	sqlite3_finalize(sqlStatement);
	std::cout <<"State updated for " << device << "/" << field <<" : " << value << '\n';
}
std::vector<std::string> DBHandler::getRuleIDs(std::string device,std::string field)
{
	std::string sql;
	std::vector<std::string> results;
		sqlite3_stmt *sqlStatement;
		sql = "SELECT rule_id FROM rule_constraint WHERE field_id ='" + field + "';";
		std::cout <<"SQL Statement: " << sql << '\n';
		if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
		{
			while(sqlite3_step(sqlStatement) == SQLITE_ROW)
			{
				results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			}
		}
		else
			std::cout <<"sqlite Prepare fail" << '\n';
		std::cout << "Results: ";
		for(std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
		    std::cout << "[" << *it << "] ";
		}
		std::cout << "\n";
		sqlite3_finalize(sqlStatement);
		return results;
}
std::vector<std::string> DBHandler::getConstraintIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT constraint_id FROM rule_constraint WHERE rule_id=" + ruleID + ";";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	std::cout << "Results: ";
	for(std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
	    std::cout << "[" << *it << "] ";
	}
	std::cout << "\n";
	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getConstraint(std::string constraintID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT field_id, constraint_operator, constraint_value FROM rule_constraint WHERE constraint_id=" + constraintID + ";";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 1)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 2)));
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	std::cout << "Results: ";
	for(std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
	    std::cout << "[" << *it << "] ";
	}
	std::cout << "\n";
	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getActionIDs(std::string ruleID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT action_id FROM rule_action WHERE rule_id=" + ruleID + ";";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		while(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	std::cout << "Results: ";
	for(std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
	    std::cout << "[" << *it << "] ";
	}
	std::cout << "\n";
	sqlite3_finalize(sqlStatement);
	return results;
}
std::vector<std::string> DBHandler::getAction(std::string actionID)
{
	std::string sql;
	std::vector<std::string> results;
	sqlite3_stmt *sqlStatement;
	sql = "SELECT rule_action.field_id, field.device_id, rule_action.action_operator, rule_action.action_value FROM rule_action INNER JOIN field ON field.field_id=rule_action.field_id WHERE action_id=" + actionID + ";";
	std::cout <<"SQL Statement: " << sql << '\n';
	if(sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, 0) == SQLITE_OK)
	{
		if(sqlite3_step(sqlStatement) == SQLITE_ROW)
		{
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 0)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 1)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 2)));
			results.push_back(std::string((char*)sqlite3_column_text(sqlStatement, 3)));
		}
	}
	else
		std::cout <<"sqlite Prepare fail" << '\n';
	std::cout << "Results: ";
	for(std::vector<std::string>::iterator it = results.begin(); it != results.end(); ++it) {
	    std::cout << "[" << *it << "] ";
	}
	std::cout << "\n";
	sqlite3_finalize(sqlStatement);
	return results;
}
void DBHandler::closeDB()
{
	sqlite3_close(db);
	std::cout <<"Closed DB connection " << '\n';
}


DBHandler::DBHandler() {
	 //sqlite3 *db;
	int  rc;
   /* Open database */
	   rc = sqlite3_open("test6.db", &db);
	   if( rc ){
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	   }else{
	      fprintf(stdout, "Opened database successfully\n");
	   }
}

DBHandler::~DBHandler() {
	//sqlite3_close(db);
	//std::cout <<"Closed DB connection " << '\n';
	// TODO Auto-generated destructor stub
}

