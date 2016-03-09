/*
 * MQTTHandler.h
 *
 *  Created on: 8 Mar 2016
 *      Author: flo
 */

#ifndef MQTTHANDLER_H_
#define MQTTHANDLER_H_

extern "C" {
#include "MQTTAsync.h"
}
#include "string.h"

class MQTTHandler {

private:

	static bool connectedStatic;
	//callbacks
	static void cb_ConnectSuccess(void* context, MQTTAsync_successData* response);
	static void cb_ConnectFailure(void* context, MQTTAsync_failureData* response);
	static void cb_ConnectionLost(void *context, char *cause);
	static void cb_SubscribeSuccess(void* context, MQTTAsync_successData* response);
	static void cb_SubscribeFailure(void* context, MQTTAsync_failureData* response);
	static int cb_MessageArrived(void *context, char *topicName, int topicLength, MQTTAsync_message *message);


public:
	MQTTHandler();
	bool getConnectionStatus();
	virtual ~MQTTHandler(); // auto created research reason for it
};
#endif /* MQTTHANDLER_H_ */
