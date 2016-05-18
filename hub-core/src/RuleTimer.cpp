/*
 * Timer.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: flo
 */

#include "RuleTimer.h"
#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include "DBHandler.h"
#include "spdlog/spdlog.h"
#include "DBHandlerMYSQL.h"
RuleTimer::RuleTimer(int inSleepTime,RuleBuffer *ruleBufferPointer,MQTTMessageBuffer *outBufferPointer) {
	logger = spdlog::get("TIMER");
	normalSleepTime = inSleepTime;
	ruleBuffer = ruleBufferPointer;
	outBuffer = outBufferPointer;
}
void RuleTimer::startTimer(){
	logger->info() << "Rule firing timer started";
	std::vector<std::array<int,2>> ruleTimes;

	while(true)
	{
		int sleepTime = normalSleepTime;
		//GET TIME
		time_t currentTime;
		struct tm *timeData;
		time (&currentTime);
		timeData = localtime (&currentTime);
		int secondsOfDay = (timeData->tm_hour * (60 * 60)) + (timeData->tm_min * 60) + timeData->tm_sec;
		logger->trace() << "Awake at: " << secondsOfDay << " Seconds";

		while (ruleTimes.size() > 0 && ruleTimes.back().operator [](1) <= secondsOfDay)
		{
			int ruleID = ruleTimes.back().operator [](0);
			logger->info() << "Rule timer firing on ruleID" << ruleID;
			Rule rule(std::to_string(ruleID));
			ruleBuffer->add(rule);
			ruleTimes.pop_back();
		}

		DBHandlerMYSQL db;
		ruleTimes = db.getTimerRules(secondsOfDay + 1,secondsOfDay + sleepTime);
		db.closeDB();
		if (ruleTimes.size() > 0)
		{
			logger->trace() << "timer constraints found within next" << sleepTime << " seconds";
			int tilNextFire = ruleTimes.back().operator [](1) - secondsOfDay;
			sleepTime = tilNextFire;
		}
		//NEW DAY FIX
		if(secondsOfDay + sleepTime >= ((24 * 60 * 60)))
		{
			logger->trace() << "New day calculation fix";
			sleepTime = (24 * 60 * 60) - secondsOfDay;
			DBHandlerMYSQL db;
			ruleTimes = db.getTimerRules(0,normalSleepTime);
			db.closeDB();
		}
		logger->trace() << "Sleeping for:" << sleepTime << " Seconds";
		usleep(sleepTime * 1000000);
	}
}
void RuleTimer::ruleBufferResolution(){
	while (true)
	{
		Rule rule = ruleBuffer->remove();
		rule.loadRule();
		if(rule.resolveRule() == true)
			rule.commitActions(outBuffer);
	}

}

RuleTimer::~RuleTimer() {
	// TODO Auto-generated destructor stub
}

