/***********************************************************************
 * Filename: wifi_ctrl.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the WiFiCtrl class for managing WiFi connections and
 *     settings, including automatic reconnection, access point 
 *     management, and network scanning.
 *
 ***********************************************************************/


#pragma once
#include "WiFi.h"
#include "parameters.h"

#define WIFI_CONTROL_TASK_PERIOD_MS 1000

#define WIFI_RECONNECT_DELAY_S 120
#define WIFI_ATTEMPT_TIMEOUT_S 20

#define WIFI_AP_TIMEOUT_S 600

#define NTP_SYNC_PERIOD_S (1 * 60 * 60)

class WiFiCtrl
{
private:
    static IPAddress apLocalIP;
    static IPAddress apGateway;
    static IPAddress apSubnet;

    static String ntpServer;

    static uint32_t attemptTimeoutTimer;
    static uint32_t apTimeoutTimer;

    static int32_t lastChannel;

    static string_reg *const scanParSSID[];

    static int16_reg *const scanParRSSI[];

    static void Scan(void);
    static void Event(WiFiEvent_t event);

public:
    static void Init(void);
    static void Task(void);
};