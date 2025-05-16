/***********************************************************************
 * Filename: device_manager.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the DeviceManager class, which serves as the server in 
 *     an ESP-NOW network. It manages connected clients, handles OTA 
 *     updates, and facilitates communication between devices. The class 
 *     provides methods to initialize, manage devices, handle parameter 
 *     updates, and handle various message types.
 *
 ***********************************************************************/


#pragma once

#include <vector>
#include <memory>
#include "Arduino.h"
#include "parameters.h"
#include "log.h"
#include "esp_now_ctrl.h"

#define COMMUNICATION_TIMEOUT_S 60

class ParameterWrapper
{
public:
    pardef_t pd;
    Register *reg = NULL;
    bool changed = false;

    static Register *createParameter(const pardef_t &pd)
    {
        uint32_t par_type = pd.dsc & 0xFF00;
        if (pd.atr & CHART_FLAG)
        {
            if (par_type == Par_U16)
            {
                return new chart_reg_dev<uint16_t>(pd);
            }
            else
            {
                return new chart_reg_dev<int16_t>(pd);
            }
        }

        switch (par_type)
        {
        case Par_S16:
            return new int16_reg(pd);
        case Par_U16:
            return new uint16_reg(pd);
        case Par_S32:
            return new int32_reg(pd);
        case Par_U32:
            return NULL;
        case Par_STRING:
            return new string_reg(pd);
        default:
            return NULL;
        }
    }

    void UpdateDefinitions(const pardef_t_espnow &pd_esp_now)
    {
        bool changed = false;

        uint32_t par_type_current = pd.dsc & 0xFF00;
        uint32_t par_type_new = pd_esp_now.dsc & 0xFF00;

        changed |= (par_type_current != par_type_new);
        if (pd_esp_now.atr & CHART_FLAG)
        {
            changed |= pd_esp_now.atr != pd.atr;
            changed |= pd_esp_now.max != pd.max;
        }

        pd.min = pd_esp_now.min;
        pd.max = pd_esp_now.max;
        pd.def = pd_esp_now.min;
        pd.dsc = (ParDscr_t)pd_esp_now.dsc;
        pd.adr = pd_esp_now.adr;
        pd.atr = pd_esp_now.atr;

        char *txt = (char *)pd.ptxt;
        strncpy(txt, pd_esp_now.ptxt, sizeof(pardef_t_espnow::ptxt));
        txt[sizeof(pardef_t_espnow::ptxt) - 1] = '\0';

        if (changed)
        {
            delete reg;
            reg = createParameter(pd);
            reg->ResetVal();
        }
    }

    void GetJson(JsonObject obj, bool for_saving = false)
    {
        obj["min"] = pd.min;
        obj["max"] = pd.max;
        obj["dsc"] = pd.dsc;
        obj["adr"] = pd.adr;
        obj["atr"] = pd.atr;
        obj["str"] = pd.ptxt;

        if (!for_saving)
        {
            reg->GetJsonVal(obj["val"].to<JsonVariant>());
        }
    }

    ParameterWrapper(const pardef_t_espnow &pd_esp_now)
    {
        char *txt = new char[sizeof(pardef_t_espnow::ptxt)];
        strncpy(txt, pd_esp_now.ptxt, sizeof(pardef_t_espnow::ptxt));
        txt[sizeof(pardef_t_espnow::ptxt) - 1] = '\0';
        pd.ptxt = txt;

        pd.min = pd_esp_now.min;
        pd.max = pd_esp_now.max;
        pd.def = pd_esp_now.min;
        pd.dsc = (ParDscr_t)pd_esp_now.dsc;
        pd.adr = pd_esp_now.adr;
        pd.atr = pd_esp_now.atr;

        reg = createParameter(pd);
        reg->ResetVal();
    }

    ~ParameterWrapper()
    {
        delete[] pd.ptxt;
        delete reg;
    }

    ParameterWrapper(const ParameterWrapper &) = delete;
    ParameterWrapper &operator=(const ParameterWrapper &) = delete;
};

class Device
{
public:
    uint8_t macAddress[6];
    PairingState_t pairState;
    DeviceType_t deviceType;
    String deviceName;
    uint32_t lastCommunication;
    uint32_t nextCommunication;
    std::vector<std::unique_ptr<ParameterWrapper>> parameters;
    uint8_t *fwUpdateData;
    uint32_t fwDataCap;
    bool fwUpdateRequested;
    bool locked;
    uint32_t fwSize;
    Log_t logs[50];
    size_t logCount;

    Device(DeviceType_t deviceType, const uint8_t *macAddr) : deviceType(deviceType), pairState(PAIR_STATE_INITIAL_REQUEST), fwUpdateRequested(false), fwUpdateData(NULL), fwSize(0), fwDataCap(0), locked(false), lastCommunication(0), nextCommunication(0), logCount(0)  
    {
        setMacAddress(macAddr);
        ESPNowCtrl::AddPeer(macAddress, 0);
    }

    virtual ~Device()
    {
        ESPNowCtrl::DeletePeer(macAddress);
        if (fwUpdateData != NULL)
        {
            free(fwUpdateData);
        }
    }

    void setMacAddress(const uint8_t *addr);

    bool compareMacAddress(const uint8_t *addr);

    uint8_t getDeviceType(void);

    ParameterWrapper *getParameter(uint16_t addr);

    ParameterWrapper *getParameter(const char *name);

    ParameterWrapper *getParameterRegister(uint16_t addr);

    void addParameter(const pardef_t_espnow &pd_esp_now);

    uint8_t setRegisterVal(uint16_t addr, int16_t val);

    void GetDeviceJson(JsonObject obj, bool for_saving = false);

    void GetParametersJson(JsonArray arr, bool for_saving = false);

    bool ParameterJsonRead(const String &name, JsonObject doc);

    bool SetParametersJson(JsonArray arr);

    bool SetParameterJson(JsonObject par);

    void PairDevice(String &name);

    size_t GetLogsJson(size_t pos, size_t nmr, JsonArray arr);

    uint32_t GetCommunicationPeriod(void);

    uint32_t GetFWVersion(void);

    bool FWUpdateBegin(size_t len);

    bool FWUpdateWrite(size_t index, uint8_t *data, size_t len, bool final);

    bool FWUpdateEnd();

    virtual void handleByteStream(const ByteStreamPayload *payload)
    {
    }
};

class FeederDevice : public Device
{
public:
    FeederDevice(DeviceType_t deviceType, const uint8_t *macAddr) : Device(deviceType, macAddr)
    {
        deviceName = "Krmitko_nove";
    }
};

class CameraDevice : public Device
{
public:
    uint8_t *picture_buf; /*!< Pointer to the pixel data */
    uint32_t picture_buf_cap;
    uint32_t picture_size;
    uint32_t tmp_picture_size;
    time_t timeStamp;

    CameraDevice(DeviceType_t deviceType, const uint8_t *macAddr) : Device(deviceType, macAddr), picture_buf(NULL), picture_buf_cap(0), picture_size(0), timeStamp(0)
    {
        deviceName = "Kamera_nova";
    }

    virtual ~CameraDevice()
    {
        if (picture_buf != NULL)
        {
            free(picture_buf);
            picture_buf = NULL;
            picture_buf_cap = 0;
            picture_size = 0;
        }
    }

    virtual void handleByteStream(const ByteStreamPayload *payload);
};

class EggCameraDevice : public CameraDevice
{
public:
    EggCameraDevice(DeviceType_t deviceType, const uint8_t *macAddr) : CameraDevice(deviceType, macAddr)
    {
        deviceName = "Kamera_AI";
    }
};

class DeviceManager
{
private:
    static int32_t updateDeviceId;

    static uint32_t saveTimer;

    static void applyPendingChanges(const uint8_t *mac_addr);

    static void paramDefsResponseHandler(const uint8_t *mac_addr, const ParamDefsPayload *payload);

    static void paramReadResponseHandler(const uint8_t *mac_addr, const ReadResponsePayload *payload);

    static void handleDataReceived(const uint8_t *mac_addr, const Message *msg, int len);

    static void onPairingRequest(const uint8_t *mac_addr, const PairRequestPayload *payload);

    static void timeSyncRequestHandler(const uint8_t *mac_addr);

    static void logResponseHandler(const uint8_t *mac_addr, const DataPayload *payload);

    static void deviceSleepHandler(const uint8_t *mac_addr, const SleepPayload *payload);

    static void byteStreamHandler(const uint8_t *mac_addr, const ByteStreamPayload *payload);

public:
    static std::mutex mutex;

    static void Init();

    static void Task();

    static Device *CreateDevice(DeviceType_t deviceType, const uint8_t *macAddr);

    static Device *GetDeviceByMac(const uint8_t *mac_addr);

    static Device *GetDeviceById(uint16_t id);

    static size_t GetDeviceCount(void);

    static void RemoveDeviceByMAC(const uint8_t *mac_addr);

    static void GetDevicesJson(JsonArray arr);

    static void GetDeviceParameters(uint16_t id, JsonArray arr);

    static bool GetDeviceParameter(uint16_t id, const String &name, JsonObject doc);

    static bool GetDeviceParameterLocked(uint16_t id, const String &name, JsonObject doc);

    static bool SetDeviceParameters(uint16_t id, JsonArray arr);

    static bool SetDeviceParameter(uint16_t id, JsonObject par);

    static void PairDeviceById(uint16_t id, String &name);

    static void RemoveDeviceById(uint16_t id);

    static bool UpdateDeviceBegin(uint16_t id, size_t len);

    static bool UpdateDeviceWrite(size_t index, uint8_t *data, size_t len, bool final);

    static void SaveDevicesToJson(void);

    static size_t GetDeviceLogs(size_t id, size_t pos, size_t nmr, JsonArray arr);

    static uint32_t GetDeviceArraySize(size_t id);

    static size_t ReadDeviceArrayBytes(size_t id, uint8_t *buf, size_t maxLen, size_t index);

private:
    static std::vector<std::unique_ptr<Device>> devices;
};
