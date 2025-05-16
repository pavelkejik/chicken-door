/***********************************************************************
 * Filename: device_manager.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the DeviceManager class a Device class.
 *
 ***********************************************************************/

#include "device_manager.h"

#define TIME_SCHEDULE(_t_secs) ((uint32_t)((_t_secs) * 1000 / COMMON_LOOP_TASK_PERIOD_MS))

std::vector<std::unique_ptr<Device>> DeviceManager::devices; // List of devices
std::mutex DeviceManager::mutex;
int32_t DeviceManager::updateDeviceId;
uint32_t DeviceManager::saveTimer;

void Device::setMacAddress(const uint8_t *addr)
{
    memcpy(macAddress, addr, 6);
}

bool Device::compareMacAddress(const uint8_t *addr)
{
    return memcmp(macAddress, addr, 6) == 0;
}

uint8_t Device::getDeviceType()
{
    return deviceType;
}

ParameterWrapper *Device::getParameter(uint16_t addr)
{
    for (auto &par : parameters)
    {
        if (par->pd.adr == addr)
        {
            return par.get();
        }
    }
    return NULL;
}

ParameterWrapper *Device::getParameter(const char *name)
{
    for (auto &par : parameters)
    {
        if (strcmp(par->pd.ptxt, name) == 0)
        {
            return par.get();
        }
    }
    return NULL;
}

ParameterWrapper *Device::getParameterRegister(uint16_t addr)
{
    for (auto &par : parameters)
    {
        if ((addr >= par->pd.adr) && (addr < (par->pd.adr + par->reg->GetSize())))
        {
            return par.get();
        }
    }
    return NULL;
}

void Device::addParameter(const pardef_t_espnow &pd_esp_now)
{
    bool exists = false;
    ParameterWrapper *existingParam = getParameter(pd_esp_now.adr);
    if (existingParam)
    {
        existingParam->UpdateDefinitions(pd_esp_now);
    }
    else
    {
        ParameterWrapper *param = new ParameterWrapper(pd_esp_now);
        if (param)
        {
            parameters.push_back(std::unique_ptr<ParameterWrapper>(param));
        }
    }
}

uint8_t Device::setRegisterVal(uint16_t addr, int16_t val)
{
    uint8_t nmr = 0;
    ParameterWrapper *pReg = getParameterRegister(addr);
    if (pReg && !pReg->changed)
    {
        nmr = pReg->reg->SetRegVal(val, addr);
    }
    return nmr;
}

void Device::GetDeviceJson(JsonObject obj, bool for_saving)
{
    obj["mac"] = MacToString(macAddress);
    obj["pair"] = pairState;
    obj["type"] = deviceType;
    obj["name"] = deviceName;

    if (!for_saving)
    {
        uint32_t period = GetCommunicationPeriod();
        obj["on"] = (lastCommunication != 0) && (lastCommunication + (3 * period * 1000)) > millis() ? true : false;
        if (lastCommunication != 0)
        {
            obj["last_com"] = (millis() - lastCommunication) / 1000;
        }
        obj["fw"] = GetFWVersion();
        obj["com"] = period;

        if (nextCommunication == 0)
        {
            obj["sleep"] = -1;
        }
        else
        {
            if (millis() > nextCommunication)
            {
                obj["sleep"] = 0;
            }
            else
            {
                obj["sleep"] = (nextCommunication - millis()) / 1000;
            }
        }
    }
}

void Device::GetParametersJson(JsonArray arr, bool for_saving)
{
    for (auto &par : parameters)
    {
        JsonObject parObj = arr.add<JsonObject>();
        par->GetJson(parObj, for_saving);
    }
}

bool Device::ParameterJsonRead(const String &name, JsonObject doc)
{
    ParameterWrapper *par = getParameter(name.c_str());
    JsonVariant var = doc[name].to<JsonVariant>();
    if (par != NULL && par->reg->IsReadable())
    {
        par->reg->GetJsonVal(var);
        return true;
    }
    return false;
}

bool Device::SetParametersJson(JsonArray arr)
{
    bool retval = true;

    for (JsonObject par : arr)
    {
        retval &= SetParameterJson(par);
    }
    return retval;
}

bool Device::SetParameterJson(JsonObject par)
{
    bool retval = true;
    uint16_t addr;

    ParameterWrapper *wPar = NULL;

    if (par.containsKey("adr"))
    {
        if (par["adr"].is<String>())
        {
            String tmp_string = par["adr"].as<String>();
            addr = tmp_string.toInt();
        }
        else
        {
            addr = par["adr"].as<uint16_t>();
        }
        wPar = getParameter(addr);
    }
    else if (par.containsKey("name") && par["name"].is<String>())
    {
        wPar = getParameter(par["name"].as<const char *>());
    }

    if (wPar != NULL && wPar->reg->IsWritable())
    {
        bool res = wPar->reg->SetJsonVal(par["val"]);
        if (res)
        {
            wPar->changed = true;
        }
        else
        {
        }
        retval &= res;
    }
    else
    {
        retval = false;
    }

    return retval;
}

void Device::PairDevice(String &name)
{
    if (pairState < PAIR_STATE_APPROVED)
    {
        deviceName = name;
        pairState = PAIR_STATE_APPROVED;
    }
}

size_t Device::GetLogsJson(size_t pos, size_t nmr, JsonArray arr)
{
    if (logCount == 0 || pos >= logCount)
    {
        return logCount;
    }
    size_t startIdx = logCount - 1 - pos;
    size_t total_nmr = std::min(startIdx + 1, nmr);

    for (size_t i = 0; i < total_nmr; ++i)
    {
        JsonObject logObj = arr.add<JsonObject>();
        logObj["v"] = logs[startIdx - i].lvl;
        logObj["t"] = logs[startIdx - i].time;
        logObj["msg"] = logs[startIdx - i].log_txt;
    }
    return logCount;
}

uint32_t Device::GetCommunicationPeriod(void)
{
    for (auto &par : parameters)
    {
        if (par->pd.atr & COMM_PERIOD_FLAG)
        {
            int16_t tmp;
            par->reg->GetRegVal(&tmp, par->pd.adr);
            return (uint32_t)tmp;
        }
    }
    return 0;
}

uint32_t Device::GetFWVersion(void)
{
    for (auto &par : parameters)
    {
        if (par->pd.atr & FW_VERSION_FLAG)
        {
            int16_t tmp;
            par->reg->GetRegVal(&tmp, par->pd.adr);
            return (uint32_t)tmp;
        }
    }
    return 0;
}

bool Device::FWUpdateBegin(size_t len)
{
    if (fwUpdateRequested || fwUpdateData != NULL)
    {
        SystemLog::PutLog("Nelze zahajit aktualizaci FW zarizeni, jiz probiha", v_error);
        return false;
    }
    else
    {
        fwUpdateData = (uint8_t *)ps_malloc(len + 1);
        if (fwUpdateData == NULL)
        {
            SystemLog::PutLog("Nelze zahajit aktualizaci FW zarizeni, neni volna pamet", v_error);
            return false;
        }
        fwDataCap = len;
        fwSize = 0;
    }
    return true;
}

bool Device::FWUpdateWrite(size_t index, uint8_t *data, size_t len, bool final)
{
    if ((index + len) <= fwDataCap && (fwUpdateData != NULL))
    {
        memcpy(fwUpdateData + index, data, len);
        fwSize += len;
        if (final)
        {
            fwUpdateRequested = true;
        }
        return true;
    }
    else
    {
        SystemLog::PutLog("Chybna velikost souboru FW", v_error);
        return false;
    }
}

void CameraDevice::handleByteStream(const ByteStreamPayload *payload)
{
    if (payload->data.index == 0)
    {
        tmp_picture_size = 0;
        if (picture_buf_cap < payload->max_mr_bytes)
        {
            if (picture_buf != NULL)
            {
                picture_buf = (uint8_t *)ps_realloc(picture_buf, payload->max_mr_bytes + 1000);
            }
            else
            {
                picture_buf = (uint8_t *)ps_malloc(payload->max_mr_bytes + 1000);
            }
            if (picture_buf == NULL)
            {
                picture_buf_cap = 0;
                return;
            }
            picture_buf_cap = payload->max_mr_bytes + 1000;
        }
    }
    if (picture_buf == NULL)
    {
        return;
    }

    if ((payload->data.index + payload->data.nmr) < picture_buf_cap)
    {
        memcpy(picture_buf + payload->data.index, payload->data.data, payload->data.nmr);
        tmp_picture_size += payload->data.nmr;
    }

    if ((payload->data.index + payload->data.nmr) >= payload->max_mr_bytes)
    {
        timeStamp = Now();
        picture_size = tmp_picture_size;
    }
}

bool Device::FWUpdateEnd()
{
    if (fwUpdateData != NULL)
    {
        free(fwUpdateData);
        fwSize = 0;
        fwDataCap = 0;
        fwUpdateData = NULL;
        fwUpdateRequested = false;
        SystemLog::PutLog("Firmware zarizeni byl uspesne aktualizovan", v_info);
        return true;
    }
    return false;
}

void DeviceManager::applyPendingChanges(const uint8_t *mac_addr)
{
    std::unique_lock<std::mutex> lock(mutex);

    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        dev->lastCommunication = millis();
        dev->nextCommunication = 0;
        for (auto &par : dev->parameters)
        {
            if (par->changed)
            {
                WriteRequestPayload payload;
                payload.regAddr = par->pd.adr;
                payload.nmr = par->reg->GetSize();
                for (int i = 0; i < payload.nmr; i++)
                {
                    par->reg->GetRegVal(&payload.values[i], payload.regAddr + i);
                }
                if (ESPNowCtrl::SendMessage(mac_addr, MSG_WRITE_PARAM_REQUEST, payload, 4 + payload.nmr * 2))
                {
                    par->changed = false;
                }
            }
        }

        if (dev->fwUpdateRequested)
        {
            dev->locked = true;
            uint32_t fwSize = dev->fwSize;
            uint8_t *updateData = dev->fwUpdateData;
            lock.unlock();
            uint8_t max_data_len = sizeof(UpdateRequestPayload::data);
            UpdateRequestPayload payload;
            for (uint32_t pos = 0; pos < fwSize; pos += max_data_len)
            {
                payload.index = pos;
                payload.isFW = 1;

                uint8_t nmr;

                if ((pos + max_data_len) < fwSize)
                {
                    nmr = max_data_len;
                    payload.isFinal = 0;
                }
                else
                {
                    nmr = fwSize - pos;
                    payload.isFinal = 1;
                }
                payload.nmr = nmr;
                memcpy(payload.data, updateData + pos, nmr);

                if (!ESPNowCtrl::SendMessage(mac_addr, MSG_FW_UPDATE_REQUEST, payload, sizeof(UpdateRequestPayload) - max_data_len + nmr, 10))
                {
                    SystemLog::PutLog("Pri aktualizaci firmwaru zarizeni doslo k chybe, pos: " + String(pos), v_error);
                    dev->locked = false;
                    break;
                }
                delay(2);

                if (payload.isFinal)
                {
                    lock.lock();
                    dev->FWUpdateEnd();
                    dev->locked = false;
                    delay(1000);
                    break;
                }
            }
        }
        ESPNowCtrl::SendMessage(mac_addr, MSG_TRANSMIT_DONE);
    }
    else
    {
        ESPNowCtrl::AddPeer(mac_addr, 0);
        PairResponsePayload response;
        response.state = PAIR_STATE_EXPIRED;
        ESPNowCtrl::SendMessage(mac_addr, MSG_PAIR_RESPONSE, response, sizeof(PairResponsePayload));
        ESPNowCtrl::SendMessage(mac_addr, MSG_TRANSMIT_DONE);
        ESPNowCtrl::DeletePeer(mac_addr);
    }
}

void DeviceManager::paramDefsResponseHandler(const uint8_t *mac_addr, const ParamDefsPayload *payload)
{
    std::lock_guard<std::mutex> lock(mutex);
    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        for (int i = 0; i < payload->numParams; i++)
        {
            dev->addParameter(payload->params[i]);
        }
        StartTimer(saveTimer, TIME_SCHEDULE(3));
    }
}

void DeviceManager::deviceSleepHandler(const uint8_t *mac_addr, const SleepPayload *payload)
{
    std::lock_guard<std::mutex> lock(mutex);
    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        dev->lastCommunication = millis();
        dev->nextCommunication = millis() + (payload->sleepTime * 1000);
    }
}

void DeviceManager::paramReadResponseHandler(const uint8_t *mac_addr, const ReadResponsePayload *payload)
{
    std::lock_guard<std::mutex> lock(mutex);
    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        for (int i = 0; i < payload->nmr; i++)
        {
            dev->setRegisterVal(i + payload->regAddr, payload->values[i]);
        }
    }
}

void DeviceManager::logResponseHandler(const uint8_t *mac_addr, const DataPayload *payload)
{
    std::lock_guard<std::mutex> lock(mutex);
    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        if (payload->index == 0)
        {
            dev->logCount = 0;
        }
        if (payload->index < sizeof(dev->logs))
        {
            size_t maxCopySize = sizeof(dev->logs) - payload->index;
            size_t copySize = std::min((size_t)(payload->nmr), maxCopySize);
            memcpy((uint8_t *)(dev->logs) + payload->index, payload->data, copySize);
            dev->logCount = (payload->index + copySize) / sizeof(Log_t);
        }
    }
}

void DeviceManager::byteStreamHandler(const uint8_t *mac_addr, const ByteStreamPayload *payload)
{
    std::lock_guard<std::mutex> lock(mutex);
    Device *dev = GetDeviceByMac(mac_addr);
    if (dev)
    {
        dev->handleByteStream(payload);
    }
}

void DeviceManager::handleDataReceived(const uint8_t *mac_addr, const Message *msg, int len)
{
    switch (msg->messageType)
    {
    case MSG_PAIR_REQUEST:
        if (msg->payloadSize == sizeof(PairRequestPayload))
        {
            onPairingRequest(mac_addr, (const PairRequestPayload *)(msg->payload));
        }
        break;

    case MSG_GET_PARAM_DEFS_RESPONSE:
        paramDefsResponseHandler(mac_addr, (const ParamDefsPayload *)(msg->payload));
        break;

    case MSG_READ_PARAM_RESPONSE:
        paramReadResponseHandler(mac_addr, (const ReadResponsePayload *)(msg->payload));
        break;

    case MSG_TIME_SYNC_REQUEST:
        timeSyncRequestHandler(mac_addr);
        break;

    case MSG_GET_LOG_RESPONSE:
        logResponseHandler(mac_addr, (const DataPayload *)(msg->payload));
        break;

    case MSG_TRANSMIT_DONE:
        applyPendingChanges(mac_addr);
        break;

    case MSG_SLEEP:
        deviceSleepHandler(mac_addr, (const SleepPayload *)(msg->payload));
        break;

    case MSG_BYTE_STREAM:
        byteStreamHandler(mac_addr, (const ByteStreamPayload *)(msg->payload));
        break;

    default:
        break;
    }
}

void DeviceManager::onPairingRequest(const uint8_t *mac_addr, const PairRequestPayload *payload)
{
    PairResponsePayload response;
    response.channel = payload->channel;
    response.deviceType = DEVICE_TYPE_DOOR_CONTROL;

    {
        std::lock_guard<std::mutex> lock(mutex);

        Device *dev = GetDeviceByMac(mac_addr);
        if (dev == NULL)
        {
            dev = CreateDevice((DeviceType_t)payload->deviceType, mac_addr);
            if (dev == NULL)
            {
                return;
            }
            devices.push_back(std::unique_ptr<Device>(dev));
        }

        if (dev->pairState == PAIR_STATE_APPROVED)
        {
            dev->pairState = PAIR_STATE_PAIRED;
            StartTimer(saveTimer, TIME_SCHEDULE(3));
        }

        response.state = dev->pairState;
    }

    ESPNowCtrl::SendMessage(mac_addr, MSG_PAIR_RESPONSE, response, sizeof(PairResponsePayload));
}

void DeviceManager::timeSyncRequestHandler(const uint8_t *mac_addr)
{
    TimeSyncPayload payload;
    payload.currentTime = Now();
    payload.sunriseTime = CasVychodu.Get();
    payload.sunsetTime = CasZapadu.Get();
    String tmp = PopisCasu.Get();
    strncpy(payload.timezone, tmp.c_str(), sizeof(payload.timezone) - 1);
    ESPNowCtrl::SendMessage(mac_addr, MSG_TIME_SYNC_RESPONSE, payload, sizeof(TimeSyncPayload));
}

void DeviceManager::Init()
{
    ESPNowCtrl::SetDataReceivedCallback(handleDataReceived);
    updateDeviceId = -1;

    JsonDocument doc(&allocator);
    {
        std::lock_guard<std::mutex> file_lock(storageFS_lock);
        LoadJsonFile("/devices.json", doc);
    }

    JsonArray arr = doc["devs"].as<JsonArray>();
    std::lock_guard<std::mutex> lock(mutex);
    for (JsonObject dev : arr)
    {
        String macString = dev["mac"].as<String>();
        uint8_t mac[6];
        StringToMac(macString, mac);
        PairingState_t pair = dev["pair"].as<PairingState_t>();
        DeviceType_t type = dev["type"].as<DeviceType_t>();
        String name = dev["name"].as<String>();
        Device *newDevice = CreateDevice(type, mac);

        if (newDevice)
        {
            newDevice->deviceName = name;
            newDevice->pairState = pair;

            if (dev.containsKey("params"))
            {
                JsonArray params = dev["params"].as<JsonArray>();
                for (JsonObject param : params)
                {
                    pardef_t_espnow pd;
                    pd.min = param["min"].as<int32_t>();
                    pd.max = param["max"].as<int32_t>();
                    pd.dsc = param["dsc"].as<uint32_t>();
                    pd.adr = param["adr"].as<uint16_t>();
                    pd.atr = param["atr"].as<uint8_t>();
                    const char *jsonString = param["str"];
                    strncpy(pd.ptxt, jsonString, sizeof(pd.ptxt) - 1);
                    pd.ptxt[sizeof(pd.ptxt) - 1] = '\0';
                    newDevice->addParameter(pd);
                }
            }

            devices.push_back(std::unique_ptr<Device>(newDevice));
        }
    }
}

void DeviceManager::Task()
{
    if (EndTimer(saveTimer))
    {
        SaveDevicesToJson();
    }
}

Device *DeviceManager::CreateDevice(DeviceType_t deviceType, const uint8_t *macAddr)
{
    switch (deviceType)
    {
    case DEVICE_TYPE_FEEDER:
        return new FeederDevice(deviceType, macAddr);
    case DEVICE_TYPE_CAMERA:
        return new CameraDevice(deviceType, macAddr);
    case DEVICE_TYPE_EGG_CAMERA:
        return new EggCameraDevice(deviceType, macAddr);
    default:
        return new Device(deviceType, macAddr);
    }
}

Device *DeviceManager::GetDeviceByMac(const uint8_t *mac_addr)
{
    for (auto &device : devices)
    {
        if (device->compareMacAddress(mac_addr))
        {
            return device.get();
        }
    }
    return NULL;
}

Device *DeviceManager::GetDeviceById(uint16_t id)
{
    if (id < devices.size())
    {
        return devices[id].get();
    }
    return NULL;
}

size_t DeviceManager::GetDeviceCount(void)
{
    return devices.size();
}

void DeviceManager::RemoveDeviceByMAC(const uint8_t *mac_addr)
{
    auto it = std::remove_if(devices.begin(), devices.end(),
                             [mac_addr](const std::unique_ptr<Device> &device) -> bool
                             {
                                 return device->compareMacAddress(mac_addr) && !device->locked;
                             });

    devices.erase(it, devices.end());
    StartTimer(saveTimer, TIME_SCHEDULE(3));
}

void DeviceManager::RemoveDeviceById(uint16_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        auto it = devices.begin() + id;
        if (!it->get()->locked)
        {
            devices.erase(it);
            StartTimer(saveTimer, TIME_SCHEDULE(3));
        }
    }
}

void DeviceManager::GetDevicesJson(JsonArray arr)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto &device : devices)
    {
        JsonObject deviceObj = arr.add<JsonObject>();
        device->GetDeviceJson(deviceObj);
    }
}

uint32_t DeviceManager::GetDeviceArraySize(size_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        Device *dev = devices[id].get();
        if (dev->deviceType == DEVICE_TYPE_CAMERA)
        {
            CameraDevice *cam = (CameraDevice *)dev;
            return cam->picture_size;
        }
        else if(dev->deviceType == DEVICE_TYPE_EGG_CAMERA)
        {
            EggCameraDevice *cam = (EggCameraDevice *)dev;
            return cam->picture_size;
        }
    }
    return 0;
}

size_t DeviceManager::ReadDeviceArrayBytes(size_t id, uint8_t *buf, size_t maxLen, size_t index)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        Device *dev = devices[id].get();
        if (dev->deviceType == DEVICE_TYPE_CAMERA)
        {
            CameraDevice *cam = (CameraDevice *)dev;

            size_t bytesToSend = cam->picture_size - index;
            if (bytesToSend > maxLen)
            {
                bytesToSend = maxLen;
            }
            memcpy(buf, cam->picture_buf + index, bytesToSend);
            return bytesToSend;
        }
        else if(dev->deviceType == DEVICE_TYPE_EGG_CAMERA)
        {
            EggCameraDevice *cam = (EggCameraDevice *)dev;

            size_t bytesToSend = cam->picture_size - index;
            if (bytesToSend > maxLen)
            {
                bytesToSend = maxLen;
            }
            memcpy(buf, cam->picture_buf + index, bytesToSend);
            return bytesToSend;
        }
    }
    return 0;
}

size_t DeviceManager::GetDeviceLogs(size_t id, size_t pos, size_t nmr, JsonArray arr)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        return devices[id]->GetLogsJson(pos, nmr, arr);
    }
    return 0;
}

void DeviceManager::SaveDevicesToJson(void)
{
    JsonDocument doc(&allocator);
    JsonArray arr = doc["devs"].to<JsonArray>();
    {
        std::lock_guard<std::mutex> lock(mutex);
        for (const auto &device : devices)
        {
            if (device->pairState == PAIR_STATE_PAIRED)
            {
                JsonObject deviceObj = arr.add<JsonObject>();
                device->GetDeviceJson(deviceObj, true);
                JsonArray pars = deviceObj["params"].to<JsonArray>();
                device->GetParametersJson(pars, true);
            }
        }
    }
    std::lock_guard<std::mutex> file_lock(storageFS_lock);
    SaveJsonFile("/devices.json", doc);
}

void DeviceManager::GetDeviceParameters(uint16_t id, JsonArray arr)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        devices[id]->GetParametersJson(arr);
    }
}

bool DeviceManager::GetDeviceParameter(uint16_t id, const String &name, JsonObject doc)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        return devices[id]->ParameterJsonRead(name, doc);
    }
    return false;
}

bool DeviceManager::GetDeviceParameterLocked(uint16_t id, const String &name, JsonObject doc)
{
    if (id < devices.size())
    {
        return devices[id]->ParameterJsonRead(name, doc);
    }
    return false;
}

bool DeviceManager::SetDeviceParameters(uint16_t id, JsonArray arr)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        return devices[id]->SetParametersJson(arr);
    }
    return false;
}

bool DeviceManager::SetDeviceParameter(uint16_t id, JsonObject par)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        return devices[id]->SetParameterJson(par);
    }
    return false;
}

void DeviceManager::PairDeviceById(uint16_t id, String &name)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        devices[id]->PairDevice(name);
    }
}

bool DeviceManager::UpdateDeviceBegin(uint16_t id, size_t len)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (id < devices.size())
    {
        if (updateDeviceId != -1)
        {
            SystemLog::PutLog("Nelze zahajit aktualizaci FW zarizeni, jiz probiha", v_error);
            return false;
        }
        updateDeviceId = id;
        return devices[id]->FWUpdateBegin(len);
    }
    return false;
}

bool DeviceManager::UpdateDeviceWrite(size_t index, uint8_t *data, size_t len, bool final)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (updateDeviceId < devices.size() && updateDeviceId >= 0)
    {
        bool res = devices[updateDeviceId]->FWUpdateWrite(index, data, len, final);
        if (final)
        {
            updateDeviceId = -1;
        }
        return res;
    }
    return false;
}
