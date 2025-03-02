/***********************************************************************
 * Filename: main.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     This file serves as the entry point for the application, 
 *     setting up and managing all hardware and software components. 
 *     It initializes modules such as Modbus, MQTT, ESP-NOW, and WiFi 
 *     control, as well as handling device management, logging, and 
 *     web server operations. The program controls various tasks for 
 *     door and light management, system logging, and sensor data 
 *     monitoring, incorporating extensive use of FreeRTOS for task 
 *     management.
 *
 ***********************************************************************/


#include <Arduino.h>
#include <HardwareSerial.h>
#include "modbus_serial.h"
#include "modbus_slave.h"
#include "semphr.h"
#include "servo.h"
#include "pin_map.h"
#include "parameters.h"
#include "error.h"
#include "button.h"
#include "log.h"
#include "led.h"
#include "door_ctrl.h"
#include "light_ctrl.h"
#include "common.h"
#include <esp_now.h>
#include <WiFi.h>
#include "wifi_ctrl.h"
#include "locations.h"
#include "esp32s3/rom/rtc.h"
#include <LittleFS.h>
#include <DNSServer.h>
#include "web_server.h"
#include "esp_ota_ops.h"
#include "device_manager.h"
#include "mqtt.h"

ModbusSerial mdbSerial;
ModbusSlave mdbSlave(mdbSerial);
Motor motor(MOTOR_UP, MOTOR_DOWN, MOTOR_EN, 0);
Encoder encoder(ENCODER_A, ENCODER_B, ENCODER_PWR);
Servo servo(motor, encoder);
Button btn(BTN, INPUT);
Led LedR(LED_R);
Led LedG(LED_G);
LightCtrl lightCtrl;

int i = 0;
int j = 0;

const char *const reset_reason_txt[] = {
    "Poweron",
    "Software",
    "Watchdog",
    "Brownout",
    "External"};

void ModbusTask(void *pvParameters)
{
  while (true)
  {
    mdbSlave.Run();
    delay(1);
  }
}

void ServoTask(void *pvParameters)
{
  while (true)
  {
    servo.Run();
    delay(10);
  }
}

void EncoderTask(void *pvParameters)
{
  while (true)
  {
    encoder.Read();
    delay(ENCODER_SAMPLE_PERIOD_MS);
  }
}

void DoorControlTask(void *pvParameters)
{
  while (true)
  {
    DoorCtrl::Task();
    servo.Ctrl();
    delay(CONTROL_TASK_PERIOD_MS);
  }
}

void LedControlTask(void *pvParameters)
{
  while (true)
  {
    if (WiFiAPOn.Get())
    {
      if (WiFi.softAPgetStationNum() > 0)
      {
        LedR = 0;
        LedG = 0xFFFF;
      }
      else
      {
        LedR = 0xFFFF;
        LedG = 0xFFFF;
      }
    }
    else
    {
      switch (StavDvirka.Get())
      {
      case NeznaznamaPoloha:
        LedR = 0xF0F0;
        LedG = 0xF0F0;
        break;
      case Otevirani:
      case Zavirani:
      case UvolneniOtevirani:
      case UvolneniZavirani:
        LedG = 0x5555;
        LedR = 0;
        break;
      case OteviraniMax:
      case ZaviraniMin:
        LedG = 0x5555;
        LedR = 5;
        break;
      case Otevreno:
      case Zavreno:
        LedG = 0;
        LedR = 0;
        break;
      case PreruseniOtevirani:
      case PreruseniZavirani:
        LedG = 0x0f;
        LedR = 0x0f;
        break;
      case MeziPoloha:
      case StopZavirani:
      case StopOtevirani:
        LedG = 0xF00F;
        LedR = 0;
        break;
      case ChybaDveri:
        LedG = 0;
        LedR = 0xffff;
        break;
      default:
        break;
      }
    }
    LedR.CtrlTask();
    LedG.CtrlTask();
    delay(LED_CTRL_TIME_MS);
  }
}

void TimeControlTask(void *pvParameters)
{
  while (true)
  {
    lightCtrl.Task();
    Location::CheckLocalization();
    delay(AUTO_DETECTION_TASK_PERIOD_S * 1000);
  }
}

void WiFiControlTask(void *pvParameters)
{
  while (true)
  {
    WiFiCtrl::Task();
    delay(WIFI_CONTROL_TASK_PERIOD_MS);
  }
}

void MQTTTask(void *pvParameters)
{
  while (true)
  {
    MQTT::Task();
    delay(MQTT_TASK_PERIOD_MS);
  }
}

void AccessLvlTask(void)
{
  static uint32_t access_timer;
  if (AccessLvl.Get() == Servis)
  {
    if (millis() > (access_timer + 10 * 60 * 1000))
    {
      access_timer = millis();
      AccessLvl.Set(Uzivatel);
      Register::ActiveLevel = Par_Public;
    }
    else
    {
      AccessLvl.Set(Servis);
      Register::ActiveLevel = Par_Installer;
    }
  }
  else
  {
    access_timer = millis();
    AccessLvl.Set(Uzivatel);
    Register::ActiveLevel = Par_Public;
  }
}

void SystemLogTask(void *pvParameters)
{
  while (true)
  {
    SystemLog::Task();

    Uptime.Set((int32_t)(esp_timer_get_time() / 1000000ULL));
    PsramUsed.Set(PsramSize.Get() - (ESP.getFreePsram() / 1024));
    HeapUsed.Set(HeapSize.Get() - (ESP.getFreeHeap() / 1024));

    AccessLvlTask();

    delay(2000);
  }
}

void DataChartTask(void *pvParameters)
{
  delay(10 * 1000);
  while (true)
  {
    Teplota24H.Set(Teplota_C.Get());
    Svetlo24H.Set(Osvetleni_proc.Get());
    delay(3 * 60 * 1000);
  }
}

void ESPNowTask(void *pvParameters)
{
  while (true)
  {
    ESPNowCtrl::Task();
  }
}

void IRAM_ATTR onTimer()
{
  encoder.Read();
}

void setup()
{
  Serial.begin(115200);

  Register::InitAll();
  storageFS.begin(true, "/storage", 5, "storage");

  Svetlo24H.SetSize(512);
  Teplota24H.SetSize(512);

  mdbSlave.Init();
  servo.Init();
  encoder.SetPosition(AktualniPoloha_puls.Get());

  SystemLog::Init();
  Error::ClearAll();
  btn.Init();
  LedR.Init();
  LedG.Init();
  DoorCtrl::Init();
  Location::Init();
  lightCtrl.Init();
  WiFiCtrl::Init();
  WebServer::Init(CaptivePortal.Get() == povoleno);
  ESPNowCtrl::Init();
  DeviceManager::Init();
  MQTT::Init();

  SDKVersion.Set(ESP.getSdkVersion());
  HeapSize.Set(ESP.getHeapSize() / 1024);
  HeapUsed.Set(HeapSize.Get() - (ESP.getFreeHeap() / 1024));
  FlashSize.Set(ESP.getFlashChipSize() / 1024);
  FlashSpeed.Set(ESP.getFlashChipSpeed() / 1000000);
  CPUFreq.Set(ESP.getCpuFreqMHz());
  PsramSize.Set(ESP.getPsramSize() / 1024);
  PsramUsed.Set(PsramSize.Get() - (ESP.getFreePsram() / 1024));
  APPSize.Set((esp_ota_get_running_partition()->size / 1024));
  APPUsed.Set(ESP.getSketchSize() / 1024);
  FSSize.Set(storageFS.totalBytes() / 1024);
  FSUsed.Set(storageFS.usedBytes() / 1024);
  Uptime.Set((int32_t)(esp_timer_get_time() / 1000000ULL));
  CompDate.Set(COMPILE_DATE_TIME);

  switch (rtc_get_reset_reason(0))
  {
  case 1:
    ResetReason.Set(rst_Poweron);
    break;
  case 3:
  case 12:
    ResetReason.Set(rst_Software);
    break;
  case 4:
  case 7:
  case 8:
  case 9:
  case 11:
  case 13:
  case 16:
    ResetReason.Set(rst_Watchdog);
    break;
  case 15:
    ResetReason.Set(rst_Brownout);
    break;

  case 14:
    ResetReason.Set(rst_External);

  default:
    ResetReason.Set(rst_Unknown);
  }

  String start_txt = ">>> Start: [Hostname: " + WiFihostname.Get();
  start_txt += "] | Verze: " + String(MainRev.Get()) + " <<<";

  SystemLog::PutLog(start_txt.c_str(), v_empty);

  start_txt = "Kompilace: " + CompDate.Get();
  start_txt += " | Reset: " + String(reset_reason_txt[ResetReason.Get()]) + " (" + String(rtc_get_reset_reason(0)) + ")";

  SystemLog::PutLog(start_txt.c_str(), v_empty);

  xTaskCreateUniversal(ModbusTask, "mdbTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, -1);
  xTaskCreateUniversal(EncoderTask, "encoderTask", getArduinoLoopTaskStackSize(), NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(ServoTask, "servoTask", getArduinoLoopTaskStackSize(), NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(DoorControlTask, "doorCtrlTask", getArduinoLoopTaskStackSize(), NULL, 2, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(LedControlTask, "ledCtrlTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(TimeControlTask, "timeCtrlTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(WiFiControlTask, "wifiTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, -1);
  xTaskCreateUniversal(SystemLogTask, "logTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(DataChartTask, "chartTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(ESPNowTask, "espNowTask", getArduinoLoopTaskStackSize(), NULL, 2, NULL, 0);
  xTaskCreateUniversal(MQTTTask, "mqttTask", getArduinoLoopTaskStackSize(), NULL, 1, NULL, -1);

  // hw_timer_t *timer = NULL;
  // // inicializace časovače
  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, ENCODER_SAMPLE_PERIOD_MS * 1000, true);
  // timerAlarmEnable(timer);
}

void loop()
{
  delay(COMMON_LOOP_TASK_PERIOD_MS);
  WebServer::Task();
  DeviceManager::Task();
  btn.Read();

  if (btn.IsOnEvent())
  {
    switch (StavDvirka.Get())
    {
    case Otevreno:
    case PreruseniZavirani:
    case NeznaznamaPoloha:
    case MeziPoloha:

      DoorCtrl::EventSourced(ev_close, povel_tlacitkem);
      break;

    case Zavreno:
    case PreruseniOtevirani:
      DoorCtrl::EventSourced(ev_open, povel_tlacitkem);
      break;

    default:
      DoorCtrl::Event(ev_stop);
      break;
    }
  }

  if (btn.IsLongOnEvent())
  {
    WiFiAPOn.Set(WiFiAPOn.Get() ? 0 : 1);
  }
}