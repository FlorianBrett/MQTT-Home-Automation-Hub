/*
 * main.cpp
 *
 *  Home Automation Hub Core
 *      Author: Florian Brett
 */
#include <string>
#include <iostream>
#include <thread>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTHandler.h"
#include <unistd.h>
#include "MQTTMessageBuffer.h"
#include "MQTTMessage.h"
#include "sqlite3.h"
#include "DBHandler.h"
#include "NewState.h"
#include <fstream>
#include "RuleTimer.h"
#include "RuleBuffer.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argv[])
{
	std::vector<spdlog::sink_ptr> sinks;

	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
	sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/logfile", "txt", 23, 59));
	//TODO Something is wrong with log file sync when low amounts of traffic log file is empty
	auto logger = std::make_shared<spdlog::logger>("GLOBAL", begin(sinks), end(sinks));
	auto dbLogger = std::make_shared<spdlog::logger>("DB", begin(sinks), end(sinks));
	auto ruleLogger = std::make_shared<spdlog::logger>("RULE", begin(sinks), end(sinks));
	auto timerLogger = std::make_shared<spdlog::logger>("TIMER", begin(sinks), end(sinks));
	auto mqttLogger = std::make_shared<spdlog::logger>("MQTT", begin(sinks), end(sinks));

	spdlog::register_logger(logger);
	spdlog::register_logger(dbLogger);
	spdlog::register_logger(ruleLogger);
	spdlog::register_logger(timerLogger);
	spdlog::register_logger(mqttLogger);

    logger->notice() << "Starting hub-core...";


	std::ifstream file("database.sql");
	std::string str;
	std::string sqlString;
	while (std::getline(file, str))
	{
		sqlString += str;
		sqlString += " ";
	}
	DBHandler db;
	db.sqlExec(sqlString);
	//should update more regularly
	int globalLoggingLevel = atoi(db.loadConfig("global_logging_level").c_str());
	logger->set_level(spdlog::level::level_enum(globalLoggingLevel));

	int dbLoggingLevel = atoi(db.loadConfig("db_logging_level").c_str());
	dbLogger->set_level(spdlog::level::level_enum(dbLoggingLevel));

	int ruleLoggingLevel = atoi(db.loadConfig("rule_logging_level").c_str());
	ruleLogger->set_level(spdlog::level::level_enum(ruleLoggingLevel));

	int timerLoggingLevel = atoi(db.loadConfig("timer_logging_level").c_str());
	timerLogger->set_level(spdlog::level::level_enum(timerLoggingLevel));

	int mqttLoggingLevel = atoi(db.loadConfig("mqtt_logging_level").c_str());
	mqttLogger->set_level(spdlog::level::level_enum(mqttLoggingLevel));

	db.closeDB();

	MQTTMessageBuffer inBuffer(100);
	MQTTMessageBuffer outBuffer(100);
	RuleBuffer ruleBuffer(100);

	MQTTHandler mqttInstance{&inBuffer,&outBuffer};
	std::thread PublishThread(&MQTTHandler::publishOutBuffer,&mqttInstance);

	RuleTimer time(10,&ruleBuffer,&outBuffer);
	std::thread TimerFireThread(&RuleTimer::startTimer,&time);
	std::thread RuleBufferResolutionThread(&RuleTimer::ruleBufferResolution,&time);

	while (true)
	{
		MQTTMessage message = inBuffer.remove();
		//TODO Filter system messages(New device, re evaluate all rules
		NewState state(message,&outBuffer);
	}

}
