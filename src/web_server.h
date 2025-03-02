/***********************************************************************
 * Filename: web_server.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the WebServer class which manages HTTP requests 
 *     via an ESPAsyncWebServer. It includes methods for
 *     initializing the server, handling device parameters, system
 *     logs, firmware updates, and managing a captive portal.
 *
 ***********************************************************************/

#pragma once
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>
#include "Arduino.h"
#include "ArduinoJson.h"


#define LOCAL_IP_URL "http://192.168.1.1"
#define LOCAL_IP_ADDR (IPAddress(192, 168, 1, 1))

class WebServer {
private:

static DNSServer dnsServer;
static AsyncWebServer server;
static fs::LittleFSFS webDataFS;
static bool captivePortal;
static uint32_t restartTimer;
static bool started;

static void InitDNSServer(void);

static bool redirectmDNS(AsyncWebServerRequest *request);
static void GetParamsHandler(AsyncWebServerRequest *request);
static void SetParamsHandler(AsyncWebServerRequest *request, JsonVariant &json);
static void GetSystemLogHandler(AsyncWebServerRequest *request);
static void GetDevicesHandler(AsyncWebServerRequest *request);
static void GetDeviceParamsHandler(AsyncWebServerRequest *request);
static void GetDeviceAllParamsHandler(AsyncWebServerRequest *request);
static void GetDeviceImageHandler(AsyncWebServerRequest *request);
static void GetDeviceImageTimestampHandler(AsyncWebServerRequest *request);
static void SetDeviceParamsHandler(AsyncWebServerRequest *request, JsonVariant &json);
static void FWUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
static void FSUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
static void DeviceFWUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
static void PairDeviceHandler(AsyncWebServerRequest *request, JsonVariant &json);
static void DeleteDeviceHandler(AsyncWebServerRequest *request);
static void GetDeviceSystemLogHandler(AsyncWebServerRequest *request);


public:
    static void Init(bool useCaptivePortal = false);
    static void Begin(void);
    static void Task(void);
    static bool Started(void);
};