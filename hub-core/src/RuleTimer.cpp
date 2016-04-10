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
RuleTimer::RuleTimer(int inSleepTime,RuleBuffer *ruleBufferPointer,MQTTMessageBuffer *outBufferPointer) {
	sleepTime = inSleepTime;
	ruleBuffer = ruleBufferPointer;
	outBuffer = outBufferPointer;
}
void RuleTimer::startTimer(){
	std::vector<std::array<int,2>> times2;

	while(true)
	{
		sleepTime = 10;
		//GET TIME
		time_t currentTime;
		struct tm *timeData;
		time (&currentTime);
		timeData = localtime (&currentTime);
		int secondsOfDay = (timeData->tm_hour * (60 * 60)) + (timeData->tm_min * 60) + timeData->tm_sec;

		//std::cout << "TIme in Seconds: " << secondsOfDay << "\n";

		//fire
		while (times2.size() > 0 && times2.back().operator [](1) <= secondsOfDay)
		{
			int ruleID = times2.back().operator [](0);
			std::cout << "/nTimer Fire: ruleID:"<< ruleID << " Time: " << times2.back().operator [](1) << "\n";
			Rule rule(std::to_string(ruleID));
			ruleBuffer->add(rule);
			times2.pop_back();
		}

		DBHandler db;
		times2 = db.getTimerRules(secondsOfDay + 1,secondsOfDay + sleepTime);
		db.closeDB();
		if (times2.size() > 0)
		{
			int tilNextFire = times2.back().operator [](1) - secondsOfDay;
			if (tilNextFire < sleepTime)
			{
				sleepTime = tilNextFire;
			}
		}
		//NEW DAY FIX
		if(secondsOfDay + sleepTime >= ((24 * 60 * 60)))
		{
			sleepTime = (24 * 60 * 60) - secondsOfDay;
			DBHandler db;
			times2 = db.getTimerRules(0,sleepTime);
			db.closeDB();
		}
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

