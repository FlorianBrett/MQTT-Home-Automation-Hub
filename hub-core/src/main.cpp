/*
 * main.cpp
 *
 *  Home Automation Hub Core
 *      Author: Florian Brett
 */
#include <string>
#include <iostream>
#include <thread>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTHandler.h"
#include <unistd.h>
#include "MQTTMessageBuffer.h"
#include "MQTTMessage.h"
#include "sqlite3.h"
#include "DBHandler.h"
#include "NewState.h"

int main(int argc, char* argv[])
{
	DBHandler db;
	char* sql;
	sql = "PRAGMA journal_mode=WAL;";
	db.sqlExec(sql);

	sql = "CREATE TABLE config("  \
	         "config_id INT PRIMARY KEY  NOT NULL," \
	         "name          TEXT         NOT NULL," \
	         "value         TEXT         NOT NULL);";

	db.sqlExec(sql);

	   sql = "INSERT INTO config (config_id,name,value) "  \
	         "VALUES (1, 'mqtt_address', '127.0.0.1:1883'); " \
		 "INSERT INTO config (config_id,name,value) "  \
	         "VALUES (2, 'mqtt_username', 'avvppads'); " \
		 "INSERT INTO config (config_id,name,value) "  \
	         "VALUES (3, 'mqtt_password', 'ORn9fPTI1hgq'); " \
		 "INSERT INTO config (config_id,name,value) "  \
	         "VALUES (4, 'mqtt_client_id', 'HubClient');";

	db.sqlExec(sql);

	sql = "CREATE TABLE device( device_id TEXT PRIMARY KEY NOT NULL, device_name TEXT NOT NULL, device_description TEXT); CREATE TABLE field( field_id TEXT PRIMARY KEY NOT NULL, device_id TEXT NOT NULL, field_name TEXT NOT NULL, field_description TEXT, field_can_set BOOLEAN NOT NULL, field_value TEXT); CREATE TABLE rule( rule_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, rule_name TEXT NOT NULL, rule_description TEXT, rule_active BOOLEAN NOT NULL); CREATE TABLE rule_constraint( constraint_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, rule_id, field_id, constraint_operator TEXT NOT NULL, constraint_value TEXT NOT NULL); CREATE TABLE rule_action( action_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, rule_id, field_id, action_operator TEXT NOT NULL, action_value TEXT NOT NULL);";
	db.sqlExec(sql);
//	sql = "INSERT INTO device (device_id,device_name,device_description) VALUES ('esp1','First ESP', 'The first ESP unit in the system'); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('led1','esp1','First LED', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('switch1','esp1','First switch', 0); INSERT INTO rule (rule_name,rule_active) VALUES ('LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('LED Off',1); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (1,'switch1','=','1'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (2,'switch1','=','0'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (1,'led1','=','0'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (2,'led1','=','1');";
//	sql = "INSERT INTO device (device_id,device_name,device_description) VALUES ('esp1','First ESP', 'The first ESP unit in the system'); INSERT INTO device (device_id,device_name,device_description) VALUES ('newDevice','A new device', 'A test device for varibles'); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('value','newDevice','Varible value', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('led1','esp1','First LED', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('switch1','esp1','First switch', 0); INSERT INTO rule (rule_name,rule_active) VALUES ('LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('var LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('LED Off',1); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (1,'switch1','=','1'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (2,'switch1','=','0'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (3,'value','>','5'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (1,'led1','=','1'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (2,'led1','=','0'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (3,'led1','=','1');";
	//sql = "INSERT INTO device (device_id,device_name,device_description) VALUES ('esp1','First ESP', 'The first ESP unit in the system'); INSERT INTO device (device_id,device_name,device_description) VALUES ('newDevice','A new device', 'A test device for varibles'); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('value','newDevice','Varible value', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('led1','esp1','First LED', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('switch1','esp1','First switch', 0); INSERT INTO rule (rule_name,rule_active) VALUES ('LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('var LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('LED Off',1); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (1,'switch1','=','1'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (2,'switch1','=','0'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (3,'value','>','5'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (1,'led1','toggle','1'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (2,'led','=','0'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (3,'led1','=','1');";
	sql = "INSERT INTO device (device_id,device_name,device_description) VALUES ('esp1','First ESP', 'The first ESP unit in the system'); INSERT INTO device (device_id,device_name,device_description) VALUES ('newDevice','A new device', 'A test device for varibles'); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('value','newDevice','Varible value', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('led1','esp1','First LED', 1); INSERT INTO field (field_id,device_id,field_name,field_can_set) VALUES ('switch1','esp1','First switch', 0); INSERT INTO rule (rule_name,rule_active) VALUES ('LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('var LED On',1); INSERT INTO rule (rule_name,rule_active) VALUES ('LED Off',1); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (1,'switch1','=','1'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (2,'switch1','=','0'); INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (3,'value','>','5'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (1,'led1','toggle','1'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (2,'value','=','1'); INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (3,'led1','=','1');";
			db.sqlExec(sql);
	db.closeDB();

	MQTTMessageBuffer inBuffer(10);
	MQTTMessageBuffer outBuffer(10);

	MQTTHandler mqttInstance{&inBuffer,&outBuffer};
	std::thread PublishThread(&MQTTHandler::publishOutBuffer,&mqttInstance);

	std::cout << "Starting reflector\n";
	while (true)
	{
		//TODO Check if setup
		MQTTMessage message = inBuffer.remove();
		NewState state(message,&outBuffer);
		//if(message.getTopic() == "/esp1/switch1")
		//{
		//	MQTTMessage message2{"/esp1/led1",message.getMessage()};
		//	outBuffer.add(message2);
		//}

	}

}
