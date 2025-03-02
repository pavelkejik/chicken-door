/***********************************************************************
 * Filename: wifi_ctrl.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the WiFiCtrl class methods defined in wifi_ctrl.h.
 *
 ***********************************************************************/

#include "wifi_ctrl.h"
#include "Arduino.h"
#include "parameters.h"
#include "WiFi.h"
#include <esp_sntp.h>
#include <esp_netif.h>
#include "common.h"
#include "locations.h"
#include "web_server.h"
#include "esp_wifi.h"

#define TIME_SCHEDULE(_t_secs) ((uint32_t)((_t_secs) * 1000 / WIFI_CONTROL_TASK_PERIOD_MS))

// WiFi AP
IPAddress WiFiCtrl::apLocalIP = LOCAL_IP_ADDR;
IPAddress WiFiCtrl::apGateway = LOCAL_IP_ADDR;
IPAddress WiFiCtrl::apSubnet = IPAddress(255, 255, 255, 0);

String WiFiCtrl::ntpServer;
int32_t WiFiCtrl::lastChannel;

uint32_t WiFiCtrl::attemptTimeoutTimer;
uint32_t WiFiCtrl::apTimeoutTimer;

string_reg *const WiFiCtrl::scanParSSID[] = {
    &WiFiScanSSID1,
    &WiFiScanSSID2,
    &WiFiScanSSID3,
    &WiFiScanSSID4,
    &WiFiScanSSID5,
    &WiFiScanSSID6,
    &WiFiScanSSID7,
    NULL};

int16_reg *const WiFiCtrl::scanParRSSI[] = {
    &WiFiScanRSSI1,
    &WiFiScanRSSI2,
    &WiFiScanRSSI3,
    &WiFiScanRSSI4,
    &WiFiScanRSSI5,
    &WiFiScanRSSI6,
    &WiFiScanRSSI7,
    NULL};

void WiFiCtrl::Event(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        WiFiStatus.Set(wifi_pripojeno);
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        if (WiFiSTAOn.Get() && (WiFiStatus.Get() != wifi_odpojeno))
        {
            if (!IsTimerRunning(attemptTimeoutTimer))
            {
                StartTimer(attemptTimeoutTimer, TIME_SCHEDULE(WIFI_ATTEMPT_TIMEOUT_S));
            }

            WiFiStatus.Set(wifi_pripojovani);
        }
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        WiFiStatus.Set(wifi_pripojeno);
        LocalIP.Set(WiFi.localIP());
        Gateway.Set(WiFi.gatewayIP());
        Mask.Set(WiFi.subnetMask());
        DNS1.Set(WiFi.dnsIP(0));
        DNS2.Set(WiFi.dnsIP(1));
        WiFiRSSI.Set(WiFi.RSSI());
        lastChannel = WiFi.channel();
        esp_wifi_set_channel(lastChannel, WIFI_SECOND_CHAN_NONE);
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        // if (WiFiSTAOn.Get() && (WiFiStatus.Get() != wifi_odpojeno))
        // {
        // if (!IsTimerRunning(attemptTimeoutTimer))
        // {
        //     StartTimer(attemptTimeoutTimer, TIME_SCHEDULE(WIFI_ATTEMPT_TIMEOUT_S));
        // }
        //     WiFiStatus.Set(wifi_pripojovani);
        // }
        break;

    default:
        break;
    }
    // WiFi.removeEvent(event);
}

void WiFiCtrl::Task(void)
{
    static bool web_server_started = false;

    if (EndTimer(attemptTimeoutTimer))
    {
        switch (WiFiStatus.Get())
        {
        case wifi_pripojovani:
            if (WiFi.status() == WL_CONNECTED)
            {
                WiFiStatus.Set(wifi_pripojeno);
            }
            else
            {
                WiFiStatus.Set(wifi_odpojeno);
                delay(100);
                WiFi.disconnect(false);
                StartTimer(attemptTimeoutTimer, TIME_SCHEDULE(WIFI_RECONNECT_DELAY_S));
            }
            break;
        case wifi_odpojeno:
            WiFiStatus.Set(wifi_vypnuto);
            break;
        default:
            break;
        }
    }

    if (EndTimer(apTimeoutTimer))
    {
        if (WiFi.softAPgetStationNum() > 0)
        {
            StartTimer(apTimeoutTimer, TIME_SCHEDULE(WIFI_AP_TIMEOUT_S));
        }
        else
        {
            WiFiAPOn.Set(0);
        }
    }

    if (WiFiScanCmd.Get())
    {
        int16_t state = WiFiStatus.Get();
        WiFiStatus.Set(wifi_hledani);
        Scan();
        WiFiScanCmd.Set(0);
        if (WiFiStatus.Get() == wifi_hledani)
        {
            WiFiStatus.Set(state);
        }
    }
    if (WiFiSTAOn.Get())
    {
        if ((WiFiStatus.Get() == wifi_vypnuto) && !WiFiSSID.Get().isEmpty())
        {
            WiFiStatus.Set(wifi_pripojovani);

            delay(100);
            if (StaticIP.Get())
            {
                WiFi.config(LocalIP.GetIP(), Gateway.GetIP(), Mask.GetIP(), DNS1.GetIP(), DNS2.GetIP());
            }

            String ssid = WiFiSSID.Get();
            String pwd = WiFiHeslo.Get();
            ssid.trim();
            pwd.trim();

            if (WiFi.begin(ssid, pwd.isEmpty() ? (const char *)NULL : pwd) != WL_CONNECT_FAILED)
            {
                StartTimer(attemptTimeoutTimer, TIME_SCHEDULE(WIFI_ATTEMPT_TIMEOUT_S));
            }
            else
            {
                WiFiStatus.Set(wifi_odpojeno);
                WiFi.disconnect(false, true);
                StartTimer(attemptTimeoutTimer, TIME_SCHEDULE(WIFI_RECONNECT_DELAY_S));
            }
            delay(1000);
        }
        else if (WiFiStatus.Get() == wifi_pripojeno)
        {
            WiFiRSSI.Set(WiFi.RSSI());
        }
    }
    else
    {
        if ((WiFiStatus.Get() != wifi_vypnuto))
        {
            WiFiStatus.Set(wifi_vypnuto);
            delay(100);
            WiFi.disconnect();
        }
    }

    if (WiFiAPOn.Get())
    {
        if (!(WiFi.getMode() & WIFI_MODE_AP))
        {
            StartTimer(apTimeoutTimer, TIME_SCHEDULE(WIFI_AP_TIMEOUT_S));

            bool res = true;
            String hostname = WiFihostname.Get();
            hostname.trim();
            if (hostname.isEmpty())
            {
                hostname = "Dvirka";
            }
            res &= WiFi.softAP(hostname, "", lastChannel > 0 ? lastChannel : 1);
            delay(100);
            res &= WiFi.softAPConfig(apLocalIP, apGateway, apSubnet);
            delay(100);
            if (!res)
            {
                WiFiAPOn.Set(0);
                WiFi.softAPdisconnect(true);
            }
        }
        WiFiAPClients.Set(WiFi.softAPgetStationNum());
    }
    else
    {
        if ((WiFi.getMode() & WIFI_MODE_AP))
        {
            WiFiAPClients.Set(0);
            WiFi.softAPdisconnect(true);
            delay(100);
            // if (lastChannel > 0 && (WiFiStatus.Get() == wifi_vypnuto))
            // {
            //     esp_wifi_set_channel(lastChannel, WIFI_SECOND_CHAN_NONE);
            // }
        }
    }

    if (!WebServer::Started())
    {
        if (WiFiStatus.Get() == wifi_pripojeno || (WiFi.getMode() & WIFI_MODE_AP))
        {
            WebServer::Begin();
        }
    }
}

void timeSyncCallback(struct timeval *tv)
{
    tm tm_info = GetTime(tv->tv_sec);
    char formattedTime[20];
    strftime(formattedTime, 20, "%d.%m.%y %H:%M", &tm_info);
    NTPSyncCas.Set(formattedTime);

    sntp_set_sync_interval(NTP_SYNC_PERIOD_S * 1000);
    Location::lastTimeSync = tv->tv_sec;
}

void WiFiCtrl::Init(void)
{
    lastChannel = -1;
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(true);
    WiFi.onEvent(Event);
    String hostname = WiFihostname.Get();
    hostname.trim();
    if (hostname.isEmpty())
    {
        hostname = "dvirka";
    }
    WiFi.setHostname(hostname.c_str());
    String macAdr = WiFi.macAddress();
    MacAdresa.Set(macAdr);
    attemptTimeoutTimer = 0;

    esp_netif_init();
    sntp_set_time_sync_notification_cb(timeSyncCallback);
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    const char *ntpserver = "pool.ntp.org";
    ntpServer = NTPServer.Get();
    ntpServer.trim();
    if (ntpServer.isEmpty())
    {
        NTPServer.Set(ntpserver);
        ntpServer = String(ntpserver);
    }

    esp_sntp_setservername(0, ntpServer.c_str());
    sntp_set_sync_interval(30 * 1000);
    esp_sntp_init();
    esp_wifi_set_ps(WIFI_PS_NONE);
}

void WiFiCtrl::Scan(void)
{
    delay(100);
    int n = WiFi.scanNetworks();
    int i = 0;
    while (i < n)
    {
        if ((scanParSSID[i] == NULL) || (scanParRSSI[i] == NULL))
        {
            break;
        }
        scanParSSID[i]->Set(WiFi.SSID(i).c_str());
        scanParRSSI[i]->Set(WiFi.RSSI(i));
        i++;
    }
    while ((scanParSSID[i] != NULL) && (scanParRSSI[i] != NULL))
    {
        scanParSSID[i]->Set("");
        scanParRSSI[i]->Set(0);
        i++;
    }
    WiFi.scanDelete();
    delay(200);
}