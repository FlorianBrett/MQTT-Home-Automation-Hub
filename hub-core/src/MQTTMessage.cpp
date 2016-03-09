/*
 * MQTTMessage.cpp
 *
 *  Created on: 9 Mar 2016
 *      Author: flo
 */

#include "MQTTMessage.h"

MQTTMessage::MQTTMessage( std::string inTopic, std::string inMessage) {
	topic = inTopic;
	message = inMessage;
}
std::string MQTTMessage::getTopic()
{
	return topic;
}
std::string MQTTMessage::getMessage()
{
	return message;
}
MQTTMessage::~MQTTMessage() {
	// TODO Auto-generated destructor stub
}

