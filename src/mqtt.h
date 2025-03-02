/***********************************************************************
 * Filename: mqtt.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the MQTT class for managing MQTT communication, 
 *     including functions for initialization, message handling, and 
 *     connection checking.
 *
 ***********************************************************************/


#pragma once

#include "Arduino.h"
#include "PubSubClient.h"
#include <WiFi.h>
#include "ArduinoJson.h"
#include "device_manager.h"
#include "parameters.h"
#include <StreamUtils.h>
#include "log.h"

#define MQTT_TASK_PERIOD_MS 50

class MQTT
{
private:
    static WiFiClient espClient;
    static PubSubClient client;
    static uint32_t lastSend;

    static String buildTopic(const String &parName, uint16_t id = UINT16_MAX);

    static void publishParameter(const String &name, uint16_t id = UINT16_MAX, bool mtx_locked = false);

    static void handleMessage(char *topic, uint8_t *payload, unsigned int length);

    static void subscribe(void);

    static bool checkConnection(void);
   

public:
    static void Init(void);

    static void Task(void);
};
