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
	sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logfile", "txt", 23, 59));

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
    //auto logger = spdlog::stdout_logger_mt("hub-core", true /*use color*/);

    logger->info() << "Starting hub-core...";
    logger->set_level(spdlog::level::info); //default
   // dbLogger->set_level(spdlog::level::debug);


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
	if(db.loadConfig("global_debug_on").compare("1") == 1){
		logger->set_level(spdlog::level::debug);
	}
	db.closeDB();

	MQTTMessageBuffer inBuffer(100);
	MQTTMessageBuffer outBuffer(10);
	RuleBuffer ruleBuffer(10);

	MQTTHandler mqttInstance{&inBuffer,&outBuffer};
	std::thread PublishThread(&MQTTHandler::publishOutBuffer,&mqttInstance);

	RuleTimer time(10,&ruleBuffer,&outBuffer);
	std::thread TimerFireThread(&RuleTimer::startTimer,&time);
	std::thread RuleBufferResolutionThread(&RuleTimer::ruleBufferResolution,&time);

	std::cout << "Starting reflector\n";
	while (true)
	{
		MQTTMessage message = inBuffer.remove();
		NewState state(message,&outBuffer);
	}

}
