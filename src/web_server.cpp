/***********************************************************************
 * Filename: web_server.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the WebServer class methods defined in web_server.h.
 *
 ***********************************************************************/

#include "Arduino.h"
#include "web_server.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>
#include "parameters.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "log.h"
#include <Update.h>
#include <ESPmDNS.h>
#include "device_manager.h"

DNSServer WebServer::dnsServer;
AsyncWebServer WebServer::server(80);
fs::LittleFSFS WebServer::webDataFS;
bool WebServer::captivePortal;
uint32_t WebServer::restartTimer;
bool WebServer::started;

#define TIME_SCHEDULE(_t_secs) ((uint32_t)((_t_secs) * 1000 / COMMON_LOOP_TASK_PERIOD_MS))

const char WebRescue[] PROGMEM =
    "<!DOCTYPE html>"
    "<html><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>ESP32 Rescue Web Page</title></head>"
    "<body>"
    "<H1>ESP32 Firmware and Filesystem Update</H1>"

    // Form for firmware.bin file
    "<form method='POST' action='/api/fw_update' enctype='multipart/form-data'>"
    "<P>Load firmware file (firmware.bin)</P>"
    "<input type='file' accept='.bin' name='FIRMWARE' required>"
    "<input type='submit' value='Update Firmware'>"
    "</form>"

    // Form for littlefs.bin file
    "<form method='POST' action='/api/fs_update' enctype='multipart/form-data'>"
    "<P>Load filesystem file (littlefs.bin)</P>"
    "<input type='file' accept='.bin' name='FILESYSTEM' required>"
    "<input type='submit' value='Update Filesystem'>"
    "</form>"

    "</body></html>";

bool isIpAddress(String str)
{
    int numDots = 0;
    int prevPos = -1;
    for (int i = 0; i <= str.length(); ++i)
    {
        if (str[i] == '.' || i == str.length())
        {
            if (i - prevPos - 1 == 0 || i - prevPos - 1 > 3)
            {
                return false;
            }
            String octet = str.substring(prevPos + 1, i);
            for (char c : octet)
            {
                if (c < '0' || c > '9')
                {
                    return false;
                }
            }
            int val = octet.toInt();
            if (val < 0 || val > 255)
            {
                return false;
            }
            numDots++;
            prevPos = i;
        }
    }
    return numDots == 4;
}

void WebServer::InitDNSServer()
{
    dnsServer.setTTL(3600);
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", LOCAL_IP_ADDR);

    //*****************************************************************************
    //! Captive portal handlers
    //*****************************************************************************
    server.on("/connecttest.txt", [](AsyncWebServerRequest *request)
              { request->redirect("http://logout.net"); });
    server.on("/canonical.html", [](AsyncWebServerRequest *request)
              { request->redirect(LOCAL_IP_URL); });
    server.on("/wpad.dat", [](AsyncWebServerRequest *request)
              { request->send(404); });
    server.on("/ncsi.txt", [](AsyncWebServerRequest *request)
              { request->redirect(LOCAL_IP_URL); });
    server.on("/generate_204", [](AsyncWebServerRequest *request)
              { request->redirect(LOCAL_IP_URL); });
    server.on("/success.txt", [](AsyncWebServerRequest *request)
              { request->send(200); });
    server.on("/redirect", [](AsyncWebServerRequest *request)
              { request->redirect(LOCAL_IP_URL); });
    server.on("/hotspot-detect.html", [](AsyncWebServerRequest *request)
              { request->redirect(LOCAL_IP_URL); });
}

void WebServer::GetParamsHandler(AsyncWebServerRequest *request)
{
    int paramsNr = request->params();
    AsyncJsonResponse *response = new AsyncJsonResponse(false);
    JsonObject root = response->getRoot();

    for (int i = 0; i < paramsNr; i++)
    {
        AsyncWebParameter *par = request->getParam(i);
        if (par)
        {
            Register::JsonRead(par->name(), root);
        }
    }

    response->setLength();
    request->send(response);
}

void WebServer::SetParamsHandler(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject jsonObj = json.as<JsonObject>();
    bool res = true;
    for (auto kvp : jsonObj)
    {
        res &= Register::JsonWrite(kvp);
    }

    if (res)
        request->send(200);
    else
        request->send(400);
}

void WebServer::PairDeviceHandler(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject jsonObj = json.as<JsonObject>();
    bool res = true;
    String name = jsonObj["name"].as<String>();
    DeviceManager::PairDeviceById(jsonObj["id"].as<uint16_t>(), name);

    if (res)
        request->send(200);
    else
        request->send(400);
}

void WebServer::DeleteDeviceHandler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("id");
    if (p == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    int deviceId = atoi(p->value().c_str());
    DeviceManager::RemoveDeviceById(deviceId);

    request->send(200);
}

void WebServer::GetDeviceParamsHandler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("id");
    if (p == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    int deviceId = atoi(p->value().c_str());

    int paramsNr = request->params();
    AsyncJsonResponse *response = new AsyncJsonResponse(false);
    JsonObject root = response->getRoot();

    for (int i = 0; i < paramsNr; i++)
    {
        AsyncWebParameter *par = request->getParam(i);
        if (par && par->name() != "id")
        {
            DeviceManager::GetDeviceParameter(deviceId, par->name(), root);
        }
    }

    response->setLength();
    request->send(response);
}

void WebServer::GetDeviceAllParamsHandler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("id");
    if (p == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    int deviceId = atoi(p->value().c_str());

    AsyncJsonResponse *response = new AsyncJsonResponse(false, &allocator);
    JsonObject root = response->getRoot();
    JsonArray arr = root["params"].to<JsonArray>();

    DeviceManager::GetDeviceParameters(deviceId, arr);

    response->setLength();
    request->send(response);
}

void WebServer::GetDeviceImageHandler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("id");
    if (p == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    int deviceId = atoi(p->value().c_str());

    uint32_t size = DeviceManager::GetDeviceArraySize(deviceId);
    request->send("image/jpeg", size, [deviceId](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                  { return DeviceManager::ReadDeviceArrayBytes(deviceId, buffer, maxLen, index); });
}

void WebServer::GetDeviceImageTimestampHandler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("id");
    if (p == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    int deviceId = atoi(p->value().c_str());

    std::unique_lock<std::mutex> lock(DeviceManager::mutex);
    CameraDevice *dev = (CameraDevice *)DeviceManager::GetDeviceById(deviceId);

    if (dev == NULL)
    {
        request->send(400, "text/plain", "Device ID is missing");
        return;
    }
    AsyncJsonResponse *response = new AsyncJsonResponse(false);
    JsonObject root = response->getRoot();
    root["timestamp"] = dev->timeStamp;

    response->setLength();
    request->send(response);
}

void WebServer::SetDeviceParamsHandler(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject jsonObj = json.as<JsonObject>();
    JsonArray parameters = jsonObj["parameters"].as<JsonArray>();

    uint16_t deviceId;
    if (jsonObj["id"].is<String>())
    {
        String tmp_string = jsonObj["id"].as<String>();
        deviceId = tmp_string.toInt();
    }
    else
    {
        deviceId = jsonObj["id"].as<uint16_t>();
    }
    bool res = true;

    res &= DeviceManager::SetDeviceParameters(deviceId, parameters);

    if (res)
        request->send(200);
    else
        request->send(400);
}

void WebServer::GetSystemLogHandler(AsyncWebServerRequest *request)
{
    int paramsNr = request->params();
    AsyncJsonResponse *response = new AsyncJsonResponse(false, &allocator);
    JsonObject root = response->getRoot();
    JsonArray arr = root["logs"].to<JsonArray>();

    size_t pos = 0;
    size_t nmr = 0;
    for (int i = 0; i < paramsNr; i++)
    {
        AsyncWebParameter *par = request->getParam(i);
        if (par->name() == "pos")
        {
            pos = par->value().toInt();
        }
        else if (par->name() == "nmr")
        {
            nmr = par->value().toInt();
        }
    }

    size_t total_nmr = SystemLog::GetLogJson(arr, pos, nmr);
    root["total"] = total_nmr;

    response->setLength();
    request->send(response);
}

void WebServer::GetDeviceSystemLogHandler(AsyncWebServerRequest *request)
{
    int paramsNr = request->params();
    AsyncJsonResponse *response = new AsyncJsonResponse(false, &allocator);
    JsonObject root = response->getRoot();
    JsonArray arr = root["logs"].to<JsonArray>();

    size_t pos = 0;
    size_t nmr = 0;
    size_t id = 0;
    for (int i = 0; i < paramsNr; i++)
    {
        AsyncWebParameter *par = request->getParam(i);
        if (par->name() == "pos")
        {
            pos = par->value().toInt();
        }
        else if (par->name() == "nmr")
        {
            nmr = par->value().toInt();
        }
        else if (par->name() == "id")
        {
            id = par->value().toInt();
        }
    }

    size_t total_nmr = DeviceManager::GetDeviceLogs(id, pos, nmr, arr);
    root["total"] = total_nmr;

    response->setLength();
    request->send(response);
}

void WebServer::GetDevicesHandler(AsyncWebServerRequest *request)
{
    AsyncJsonResponse *response = new AsyncJsonResponse(false, &allocator);
    JsonObject root = response->getRoot();
    JsonArray arr = root["devices"].to<JsonArray>();

    DeviceManager::GetDevicesJson(arr);

    response->setLength();
    request->send(response);
}

void WebServer::FWUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        SystemLog::PutLog("Start aktualizace firmwaru", v_info);

        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
            Update.printError(Serial);
    }
    if (Update.write(data, len) != len)
        Update.printError(Serial);
    if (final)
    {
        if (!Update.end(true))
        {
            SystemLog::PutLog("Pri aktualizaci firmwaru doslo k chybe. Zarizeni se restartuje.", v_error);
            StartTimer(restartTimer, TIME_SCHEDULE(3));

            Update.printError(Serial);
            request->send(404);
        }
        else
        {
            SystemLog::PutLog("Aktualizace firmwaru probehla uspesne", v_info);
            request->send(200);
        }
    }
}

void WebServer::FSUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        SystemLog::PutLog("Start aktualizace souboroveho systemu", v_info);
        webDataFS.end();
        delay(50);
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS, -1, 0, "webdata"))
            Update.printError(Serial);
    }
    if (Update.write(data, len) != len)
        Update.printError(Serial);
    if (final)
    {
        if (!Update.end(true))
        {
            SystemLog::PutLog("Pri aktualizaci souboroveho systemu doslo k chybe. Zarizeni se restartuje.", v_error);
            StartTimer(restartTimer, TIME_SCHEDULE(3));
            Update.printError(Serial);
            request->send(404);
        }
        else
        {
            webDataFS.begin(false, "/web", 10, "webdata");
            delay(50);
            SystemLog::PutLog("Aktualizace souboroveho systemu probehla uspesne", v_info);
            request->send(200);
        }
    }
}

void WebServer::DeviceFWUpdateHandler(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        request->contentLength();
        AsyncWebParameter *p = request->getParam("id");
        if (p == NULL)
        {
            request->send(400, "text/plain", "Device ID is missing");
            return;
        }
        int deviceId = atoi(p->value().c_str());

        if (!DeviceManager::UpdateDeviceBegin(deviceId, request->contentLength()))
        {
            request->send(400, "text/plain", "Cannot start update");
            return;
        }
        SystemLog::PutLog("Start aktualizace firmwaru zarizeni ID: " + String(deviceId), v_info);
    }

    if (!DeviceManager::UpdateDeviceWrite(index, data, len, final))
    {
        request->send(400, "text/plain", "Error occured during update");
        return;
    }

    if (final)
    {
        request->send(200);
    }
}

bool WebServer::redirectmDNS(AsyncWebServerRequest *request)
{
    if (request->host().equalsIgnoreCase(WiFihostname.Get() + ".local"))
    {
        IPAddress localIP;
        if (request->client()->localIP() == WiFi.softAPIP())
        {
            localIP = WiFi.softAPIP();
        }
        else
        {
            localIP = WiFi.localIP();
        }
        String redirectUrl = "http://" + localIP.toString() + request->url();
        AsyncWebServerResponse *response = request->beginResponse(301, "text/plain", "Moved Permanently");
        response->addHeader("Location", redirectUrl);
        response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");

        request->send(response);

        return true;
    }
    return false;
}

void WebServer::Init(bool useCaptivePortal)
{
    started = false;
    captivePortal = useCaptivePortal;

    webDataFS.begin(false, "/web", 10, "webdata");

    if (useCaptivePortal)
    {
        InitDNSServer();
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                    if(redirectmDNS(request))
                    {
                        return;
                    }
                request->send(webDataFS, "/index.html", "text/html"); });

    server.serveStatic("/", webDataFS, "/").setDefaultFile("index.html").setCacheControl("max-age=300");

    server.on("/api/get_params", HTTP_GET, GetParamsHandler);

    server.addHandler(new AsyncCallbackJsonWebHandler(
        "/api/set_params", SetParamsHandler));

    server.addHandler(new AsyncCallbackJsonWebHandler(
        "/api/pair_device", PairDeviceHandler));

    server.on("/api/get_log", HTTP_GET, GetSystemLogHandler);

    server.on("/api/get_devices", HTTP_GET, GetDevicesHandler);

    server.on("/api/get_device_params", HTTP_GET, GetDeviceParamsHandler);

    server.on("/api/get_device_all_params", HTTP_GET, GetDeviceAllParamsHandler);

    server.on("/api/delete_device", HTTP_GET, DeleteDeviceHandler);

    server.addHandler(new AsyncCallbackJsonWebHandler(
        "/api/set_device_params", SetDeviceParamsHandler));

    server.on("/api/get_device_log", HTTP_GET, GetDeviceSystemLogHandler);

    server.on("/api/get_device_image", HTTP_GET, GetDeviceImageHandler);

    server.on("/api/get_image_timestamp", HTTP_GET, GetDeviceImageTimestampHandler);

    server.on(
        "/api/fw_update", HTTP_POST,
        [](AsyncWebServerRequest *request) {
        },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
        { FWUpdateHandler(request, filename, index, data, len, final); });

    server.on(
        "/api/fs_update", HTTP_POST,
        [](AsyncWebServerRequest *request) {
        },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
        { FSUpdateHandler(request, filename, index, data, len, final); });

    server.on(
        "/api/device_fw_update", HTTP_POST,
        [](AsyncWebServerRequest *request) {
        },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
        { DeviceFWUpdateHandler(request, filename, index, data, len, final); });

    server.on("/rescue", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", WebRescue); });

    server.on("/api/restart", HTTP_GET, [](AsyncWebServerRequest *request)
              { StartTimer(restartTimer,TIME_SCHEDULE(3));  request->send(200, "text/plain", "Device is going to reboot."); });

    // Catch-All Handlers - Redirect all requests to index.html
    // This allows the Vue router to handle the path
    server.onNotFound([](AsyncWebServerRequest *request)
                      {

  if (captivePortal && !isIpAddress(request->host()) && !request->host().equalsIgnoreCase(WiFihostname.Get()+".local")) {
        request->redirect(String("http://") + request->client()->localIP().toString());
        return;
  }

    if(redirectmDNS(request))
    {
        return;
    }

    if (request->url().indexOf('.') != -1) {
      request->send(404);
    } else {
      request->send(webDataFS, "/index.html", "text/html");
    } });
}

void WebServer::Begin(void)
{
    if (!started)
    {
        started = true;
        String hostname = WiFihostname.Get();
        hostname.trim();
        if (hostname.isEmpty())
        {
            hostname = "dvirka";
        }
        if (MDNS.begin(hostname))
        {
            MDNS.addService("http", "tcp", 80);
        }

        server.begin();
    }
}

void WebServer::Task(void)
{
    if (EndTimer(restartTimer))
    {
        ESP.restart();
    }

    if (captivePortal)
    {
        dnsServer.processNextRequest();
    }
}

bool WebServer::Started(void)
{
    return started;
}
