/*
 * MQTTMessage.h
 *
 *  Created on: 9 Mar 2016
 *      Author: flo
 */

#ifndef MQTTMESSAGE_H_
#define MQTTMESSAGE_H_

#include <string>
using std::string;
class MQTTMessage {
private:
	std::string topic;
	std::string message;
public:
	MQTTMessage(std::string inTopic,std::string inMessage);
	int getTest();
	std::string getTopic();
	std::string getMessage();
	virtual ~MQTTMessage();
};

#endif /* MQTTMESSAGE_H_ */
