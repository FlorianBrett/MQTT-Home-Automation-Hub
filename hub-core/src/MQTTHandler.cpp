/*
 * MQTTHandler.cpp
 *
 *  Created on: 8 Mar 2016
 *      Author: flo
 */

#include "MQTTHandler.h"

#include "stdlib.h"
#include <unistd.h>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
//C Includes
extern "C" {
#include "MQTTAsync.h"
}

// TODO temporary defines in future will be stored in DB
#define ADDRESS     "m10.cloudmqtt.com:10308"
#define CLIENTID    "CClient"
#define USERNAME     "avvppads"
#define PASSWORD     "ORn9fPTI1hgq"
#define TOPIC       "/esp1/led1"
#define TOPIC2       "/esp1/switch1"
#define PAYLOAD     "Hello World!"
#define PAYLOAD1     "1"
#define PAYLOAD0     "0"
// CALLBACKS

bool MQTTHandler::connectedStatic = false;
//TODO buffer pointers

// Callbacks
int MQTTHandler::cb_MessageArrived(void *context, char *topicName, int topicLength, MQTTAsync_message *message)
{
	std::cout << "Message arrived" << "\n";

    //buffer population
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}
void MQTTHandler::cb_ConnectionLost(void *context, char *cause)
{
	std::cout << "Connection lost" << "\n";
	connectedStatic = false;
	// TODO attempt reconnect
}
void MQTTHandler::cb_ConnectFailure(void* context, MQTTAsync_failureData* response)
{
	std::cout << "Connection Failure RC: " << (response ? response->code : 0) << "\n";
	//printf("Connect failed, rc %d\n", response ? response->code : 0);
}
void MQTTHandler::cb_ConnectSuccess(void* context, MQTTAsync_successData* response)
{
	std::cout << "Connection Success" << "\n";
	connectedStatic = true;
	// TODO maybe publish a hello world message?
	// TODO subscribe mqtt topics

	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	//MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	printf("Subscribing to topic %s for client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC2, CLIENTID, 1);
	opts.onSuccess = cb_SubscribeSuccess;
	opts.onFailure = cb_SubscribeFailure;
	opts.context = client;

	if ((rc = MQTTAsync_subscribe(client, TOPIC2, 1, &opts)) != MQTTASYNC_SUCCESS)
	{
		//TODO is this error catching needed with as i havea call backs?
		printf("Failed to start subscribe, return code %d\n", rc);
		std::cout << "Subscribe Failure RC: " << rc << "\n";
		//exit(-1);
	}
}
void MQTTHandler::cb_SubscribeSuccess(void* context, MQTTAsync_successData* response)
{
	std::cout << "Subscribe Success" << "\n";

}
void MQTTHandler::cb_SubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	std::cout << "Subscribe Failure RC: " << (response ? response->code : 0) << "\n";
}

//Normal Functions
bool MQTTHandler::getConnectionStatus()
{
	return connectedStatic;
}
MQTTHandler::MQTTHandler() {
	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	MQTTAsync_token token;
	int rc;

	MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	MQTTAsync_setCallbacks(client, NULL, &cb_ConnectionLost, *cb_MessageArrived, NULL);
	printf("does it reach here? 2");
	conn_opts.context = client;
	conn_opts.username = USERNAME;
	conn_opts.password = PASSWORD;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	conn_opts.onSuccess = cb_ConnectSuccess;
	conn_opts.onFailure = cb_ConnectFailure;

	printf("does it reach here? 3 ");
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		//exit(-1);
	}
}

MQTTHandler::~MQTTHandler() {
	// TODO Auto-generated destructor stub
	//MQTTAsync_destroy(&client);
}

