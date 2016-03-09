/*
 * MQTTMessageBuffer.cpp
 *
 *  Created on: 9 Mar 2016
 *      Author: flo
 */

#include "MQTTMessageBuffer.h"
#include <iostream>

MQTTMessageBuffer::MQTTMessageBuffer() {
	// TODO Auto-generated constructor stub

}
void MQTTMessageBuffer::add(MQTTMessage inMessage)
{
	std::unique_lock<std::mutex> lck (mtx);
	while (inMessageBuffer.size() >= bufferSize) cv.wait(lck);
	inMessageBuffer.push_back(inMessage);
	std::cout <<"Buffer: add Size:" << inMessageBuffer.size() << '\n';
	lck.unlock();
	cv.notify_all();
}
MQTTMessage MQTTMessageBuffer::remove()
{
	std::unique_lock<std::mutex> lck (mtx);
	while (inMessageBuffer.size() <= 0) cv.wait(lck);
	MQTTMessage outMessage = inMessageBuffer.back();
	inMessageBuffer.pop_back();
	std::cout <<"Buffer: remove Size:" << inMessageBuffer.size() << '\n';
	lck.unlock();
	cv.notify_all();
	return outMessage;
}
bool MQTTMessageBuffer::isFull()
{
	bool full = false;
	if (inMessageBuffer.size() >= bufferSize)
		full = true;
	return full;
}
MQTTMessageBuffer::~MQTTMessageBuffer() {
	// TODO Auto-generated destructor stub
}

