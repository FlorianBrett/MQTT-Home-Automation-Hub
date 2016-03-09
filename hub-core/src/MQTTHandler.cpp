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
MQTTMessageBuffer *MQTTHandler::inBuffer = NULL;
MQTTMessageBuffer *MQTTHandler::outBuffer = NULL;
//TODO buffer pointers

// Callbacks
int MQTTHandler::cb_MessageArrived(void *context, char *topicName, int topicLength, MQTTAsync_message *message)
{
	std::cout << "Message arrived" << "\n";
    string newtopic(topicName);
    char* payloadptr;
    payloadptr = (char*)message->payload;
    //string newmessage((char*)message->payload);
    string newmessage = string( payloadptr, message->payloadlen);
    MQTTMessage inMessage(newtopic,newmessage);
    if (!inBuffer->isFull())
    	inBuffer->add(inMessage);
    else
    	std::cout << "Buffer full data lost!"<< "\n";


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
void MQTTHandler::publishOutBuffer()
{
	while(true)
	{
		if(connectedStatic)
		{
			MQTTMessage message = outBuffer->remove();
			publishMessage(message);
		}
	}
}
void MQTTHandler::publishMessage(MQTTMessage message)
{

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	//printf("Message sending Topic: %s Message: %d\n",TOPIC,state);
	//std::cout << "Message sending Topic: " << TOPIC << "Message:" << outMessage <<'\n';

	//opts.onSuccess = onSend;
	opts.context = client;
	pubmsg.payload = (char*)message.getMessage().c_str();
	pubmsg.payloadlen = strlen(message.getMessage().c_str());

	pubmsg.qos = 1;
	pubmsg.retained = 0;


	if ((rc = MQTTAsync_sendMessage(client, message.getTopic().c_str(), &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage2, return code %d\n", rc);
 		//exit(-1);
	}
}
MQTTHandler::MQTTHandler(MQTTMessageBuffer *inBufferPointer, MQTTMessageBuffer *outBufferPointer) {
	inBuffer = inBufferPointer;
	outBuffer = outBufferPointer;

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	MQTTAsync_token token;
	int rc;

	MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	MQTTAsync_setCallbacks(client, NULL, &cb_ConnectionLost, *cb_MessageArrived, NULL);
	conn_opts.context = client;
	conn_opts.username = USERNAME;
	conn_opts.password = PASSWORD;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	conn_opts.onSuccess = cb_ConnectSuccess;
	conn_opts.onFailure = cb_ConnectFailure;

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

