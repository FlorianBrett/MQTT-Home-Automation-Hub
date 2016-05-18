/*
 * MQTTMessageBuffer.cpp
 *
 *  Created on: 9 Mar 2016
 *      Author: flo
 */

#include "MQTTMessageBuffer.h"
#include <iostream>

MQTTMessageBuffer::MQTTMessageBuffer(int inBufferSize) {
	bufferSize = inBufferSize;

}
void MQTTMessageBuffer::add(MQTTMessage inMessage)
{
	std::unique_lock<std::mutex> lck (mtx);
	while (messageBuffer.size() >= bufferSize) cv.wait(lck);
	messageBuffer.push_back(inMessage);
	lck.unlock();
	cv.notify_all();
}
MQTTMessage MQTTMessageBuffer::remove()
{
	std::unique_lock<std::mutex> lck (mtx);
	while (messageBuffer.size() <= 0) cv.wait(lck);
	MQTTMessage outMessage = messageBuffer.back();
	messageBuffer.pop_back();
	lck.unlock();
	cv.notify_all();
	return outMessage;
}
bool MQTTMessageBuffer::isFull()
{
	bool full = false;
	if (messageBuffer.size() >= bufferSize)
		full = true;
	return full;
}
MQTTMessageBuffer::~MQTTMessageBuffer() {
	// TODO Auto-generated destructor stub
}

