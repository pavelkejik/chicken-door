/***********************************************************************
 * Filename: mqtt.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the MQTT class methods defined in mqtt.h. Various helper 
 *     functions are implemented to manage parameter publishing, 
 *     subscriptions, and message parsing.
 *
 ***********************************************************************/

#include "mqtt.h"

WiFiClient MQTT::espClient;
PubSubClient MQTT::client(espClient);
uint32_t MQTT::lastSend;

String MQTT::buildTopic(const String &parName, uint16_t id)
{
    String baseTopic = MQTTbase.Get();
    String topic;
    if (id != UINT16_MAX)
    {
        topic = baseTopic + "/devices/" + String(id) + "/state/" + parName;
    }
    else
    {
        topic = baseTopic + "/state/" + parName;
    }
    return topic;
}

void MQTT::publishParameter(const String &name, uint16_t id, bool mtx_locked)
{
    JsonDocument tmpDoc;
    bool res = false;

    if (id != UINT16_MAX)
    {
        if (mtx_locked)
        {
            res = DeviceManager::GetDeviceParameterLocked(id, name, tmpDoc.to<JsonObject>());
        }
        else
        {
            res = DeviceManager::GetDeviceParameter(id, name, tmpDoc.to<JsonObject>());
        }
    }
    else
    {
        res = Register::JsonRead(name, tmpDoc.to<JsonObject>());
    }

    if (res)
    {
        JsonDocument doc;
        doc["value"] = tmpDoc[name];

        size_t json_size = measureJson(doc);
        if (json_size > 200)
        {

            client.beginPublish(buildTopic(name, id).c_str(), json_size, false);
            BufferingPrint bufferedClient(client, 64);
            serializeJson(doc, bufferedClient);
            bufferedClient.flush();
            client.endPublish();
        }
        else
        {
            char buffer[250];
            size_t n = serializeJson(doc, buffer);
            client.publish(buildTopic(name, id).c_str(), buffer, n);
        }
    }
}

void MQTT::handleMessage(char *topic, uint8_t *payload, unsigned int length)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    char *token;
    char *rest = topic;
    int devId = -1;
    String parameterName;
    bool isSetFound = false;

    while ((token = strtok_r(rest, "/", &rest)))
    {
        if (strcmp(token, "set") == 0)
        {
            isSetFound = true;
            token = strtok_r(rest, "/", &rest);
            if (token == NULL)
            {
                Serial.println("Error: Parameter name missing after 'set'");
                return;
            }
            parameterName = String(token);
        }
        else if (strcmp(token, "devices") == 0 && isSetFound == false)
        {
            token = strtok_r(rest, "/", &rest);
            if (token == NULL)
            {
                Serial.println("Error: Subdevice ID missing after 'devices'");
                return;
            }
            devId = atoi(token);
        }
    }

    if (!isSetFound)
    {
        Serial.println("Error: 'set' segment not found in topic");
        return;
    }

    if (doc.containsKey("value"))
    {
        doc["val"] = doc["value"];
    }
    else if (doc.containsKey("state"))
    {
        doc["val"] = doc["state"];
    }

    if (doc.containsKey("val"))
    {
        if (devId != -1)
        {
            doc["name"] = parameterName;

            if (DeviceManager::SetDeviceParameter(devId, doc.as<JsonObject>()))
            {
                publishParameter(parameterName, devId);
            }
        }
        else
        {
            if (Register::JsonWrite(parameterName, doc["val"]))
            {
                publishParameter(parameterName);
            }
        }
    }
}

void MQTT::subscribe(void)
{
    String baseTopic = MQTTbase.Get();
    String mainDeviceSetTopic = baseTopic + "/set/+";
    String subDeviceSetTopic = baseTopic + "/devices/+/set/+";
    client.subscribe(mainDeviceSetTopic.c_str());
    client.subscribe(subDeviceSetTopic.c_str());
}

bool MQTT::checkConnection()
{
    bool res = false;
    if (MQTTOn.Get())
    {
        if (WiFiStatus.Get() != wifi_pripojeno)
        {
            MQTTStatus.Set(mqtt_odpojeno);
            return false;
        }
        if (!client.connected())
        {
            MQTTStatus.Set(mqtt_pripojovani);

            Serial.print("Attempting MQTT connection...");

            String base = MQTTbase.Get();
            String name = MQTTJmeno.Get();
            String pwd = MQTTHeslo.Get();
            client.setServer(BrokerAdresa.GetIP(), BrokerPort.Get());

            if (client.connect(base.c_str(), name.isEmpty() ? NULL : name.c_str(), pwd.isEmpty() ? NULL : pwd.c_str()))
            {
                MQTTStatus.Set(mqtt_pripojeno);
                res = true;
                Serial.println("connected");
                subscribe();
            }
            else
            {
                MQTTStatus.Set(mqtt_odpojeno);
                delay(10000);
            }
        }
        else
        {
            MQTTStatus.Set(mqtt_pripojeno);
            res = true;
        }
    }
    else
    {
        if (MQTTStatus.Get() != mqtt_vypnuto)
        {
            client.disconnect();
            MQTTStatus.Set(mqtt_vypnuto);
        }
    }
    return res;
}

void MQTT::Init(void)
{
    if (MQTTbase.Get().isEmpty())
    {
        MQTTbase.Set("Dvirka");
    }
    MQTTStatus.Set(mqtt_vypnuto);
    client.setCallback(handleMessage);
    lastSend = 0;
}

void MQTT::Task(void)
{
    if (checkConnection())
    {
        client.loop();
        if ((lastSend == 0) || ((millis() - lastSend) > (MQTTInterval_S.Get() * 1000)))
        {
            lastSend = millis();

            for (int i = 0; i < Register::NmrParameters; i++)
            {
                Register *reg = Register::GetParByIdx(i);
                if (reg && (reg->def.dsc & Par_MQTT))
                {
                    publishParameter(String(reg->def.ptxt));
                }
            }
            {
                size_t devs_cnt = DeviceManager::GetDeviceCount();
                Device *dev = NULL;
                for (size_t id = 0; id < devs_cnt; id++)
                {
                    std::lock_guard<std::mutex> lock(DeviceManager::mutex);
                    Device *dev = DeviceManager::GetDeviceById(id);
                    if (!dev)
                    {
                        break;
                    }
                    for (auto &par : dev->parameters)
                    {
                        publishParameter(String(par->pd.ptxt), id, true);
                    }
                }
            }
        }
    }
}