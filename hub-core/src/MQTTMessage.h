/*
 * MQTTMessage.h
 * This file written by Florian Brett is released with the GNU Public License.
 */

#ifndef MQTTMESSAGE_H_
#define MQTTMESSAGE_H_

#include <string>
using std::string;

/**
 * A simplistic message holder class.
 * A class to hold the MQTT topic and message.

 *  @author Florian Brett
 *  @version 0.1
 *  @date Created 09/03/16
 *  @date Updated 09/04/16 Commented header file
 *  @copyright GNU Public License
 */
class MQTTMessage {
private:
	/**
	 * A string containing the MQTT topic.
	 */
	std::string topic;
	/**
	 * A string containing the MQTT message.
	 */
	std::string message;
public:
	/**
	 * MQTTMessage Constructor.
	 * @param inTopic The topic to be stored in this MQTTMessage.
     * @param inMessage The message to be stored in this MQTTMessage.
	 */
	MQTTMessage(std::string inTopic, std::string inMessage);
	/**
	 * Get accessor for topic.
	 * @returns topic for this instance of MQTTMessage.
	 */
	std::string getTopic();
	/**
	 * Get accessor for message.
	 * @returns message for this instance of MQTTMessage.
	 */
	std::string getMessage();
	/**
	 * MQTTMessage Destructor.
	 */
	virtual ~MQTTMessage();
};

#endif /* MQTTMESSAGE_H_ */
