/*
 * MQTTMessageBuffer.h
 *
 *  Created on: 9 Mar 2016
 *      Author: flo
 */

#ifndef MQTTMESSAGEBUFFER_H_
#define MQTTMESSAGEBUFFER_H_

#include <deque>
#include <mutex>
#include "MQTTMessage.h"
#include <condition_variable>
class MQTTMessageBuffer {
private:
	std::deque<MQTTMessage> inMessageBuffer;
    const unsigned int bufferSize = 10;
    std::mutex mtx;
    std::condition_variable cv;
public:
    MQTTMessageBuffer();
    void add(MQTTMessage inMessage);
    MQTTMessage remove();
    bool isFull();
	virtual ~MQTTMessageBuffer();
};

#endif /* MQTTMESSAGEBUFFER_H_ */
