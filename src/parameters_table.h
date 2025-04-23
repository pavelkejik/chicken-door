/****************************************************************************
 * Filename: parameters_table.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     This header file contains the definitions for all parameters and Modbus
 *     registers used throughout the application. It defines a structured way
 *     to declare system parameters that are essential for operational control,
 *     configuration, and status monitoring of various components.
 *
 *     The parameters are defined using the following macros that encapsulate
 *     their properties and behavioral characteristics:
 *     - DefPar_Ram: Define a volatile parameter stored in RAM.
 *     - DefPar_Nv: Define a non-volatile parameter stored in persistent memory.
 *     - DefPar_Fun: Define a custom parameter type derived from the Register class.
 *     - DefPar_Ext: Reference an existing parameter, primarily used for Modbus addressing.
 *
 *     In this table use following macros:
 *     - DefPar_Ram(_alias_,_regadr_,_def_,_min_, _max_,_type_,_dir_,_lvl_,_atr_)
 *     - DefPar_Nv (_alias_,_regadr_,_def_,_min_, _max_,_type_,_dir_,_lvl_,_atr_)
 *     - DefPar_Fun(_alias_,_regadr_,_def_,_min_, _max_,_type_,_dir_,_lvl_,_atr_,_fun_)
 *     - DefPar_Ext(_alias_,_regadr_,_def_,_min_,_max_,_type_,_dir_)
 * 
 * 
 *     Parameters are defined with attributes:
 *     - _alias_: Name of the parameter instance derived from the Register class.
 *     - _regadr_: Modbus register address. Parameters can occupy mutliple registers.
 *     - _def_: Default value, overwritten by stored value if valid for non-volatile types.
 *     - _min_, _max_: Define the permissible range of values.
 *     - _type_: Data type of the parameter, e.g., S16_, U16_, S32_, U32_, STRING_.
 *     - _dir_: Access direction (Par_R for read-only, Par_RW for read/write, Par_W for write-only).
 *     - _lvl_: Access level required to modify the parameter:
 *         - Par_Public: Accessible without privileges.
 *         - Par_Installer: Requires installer-level privileges.
 *         - Par_ESPNow: Accessible over ESP-Now, suitable for sharing definitions/values.
 *         - Par_MQTT: Accessible over MQTT, suitable for remote configuration or monitoring.
 *     - _atr_: Additional user-defined attributes or flags.
 *     - _fun_: Specific parameter type derived from the Register class (used with DefPar_Fun).
 ****************************************************************************/

#ifdef PAR_DEF_INCLUDES
#include "error.h"
#include "log.h"
#include "servo.h"
#include "door_ctrl.h"
#undef PAR_DEF_INCLUDES

#else /*PAR_DEF_INCLUDES*/

/*
-----------------------------------------------------------------------------------------------------------
  @ Stav dvirek

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Ram(StavDvirka, 1, NeznaznamaPoloha, NeznaznamaPoloha, ChybaDveri, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(DolniPoloha, 2, Nedetekovan, Nedetekovan, Overen, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(HorniPoloha, 3, Nedetekovan, Nedetekovan, Overen, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Fun(AktualniPoloha_puls, 4, 0, -1000, 1000, S16_, Par_R, Par_Public, FLAGS_NONE, enc_reg)
DefPar_Ram(AktualniRychlost, 5, 0, -1000, 1000, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(AktualniProud_mA, 6, 0, 0, 2000, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(PozadovaneOtevreni_proc, 7, 0, -100, 100, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(PozadovanaPoloha_puls, 8, 0, -1000, 1000, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(PozadovanaRychlost, 9, 0, -1000, 1000, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(RizeniMotoru_PWM, 10, 0, -100, 100, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(ErrorCode, 11, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(Otevreno_puls, 12, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(Otevreni_proc, 13, 0, 0, 100, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Fun(Teplota_C, 14, 0, -100, 100, S16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE, temp_reg)
DefPar_Ram(Osvetleni_proc, 15, 0, 0, 100, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Fun(CasOtevreni, 16, 0, 0, 6, STRING_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE, time_reg)
DefPar_Fun(CasZavreni, 19, 0, 0, 6, STRING_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE, time_reg)
DefPar_Ram(CasDoOtevreni, 22, 0, 0, 300, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(CasDoZavreni, 23, 0, 0, 300, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Ram(AutomatikaOtevreni, 24, Manualni, Manualni, Casem, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(AutomatikaZavreni, 25, Manualni, Manualni, Casem, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Ram(PovelOd, 26, povel_neni, povel_neni, povel_neznamy, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Fun(Command, 27, manual_stop, manual_stop, manual_zavrit, U16_, Par_RW, Par_Public | Par_MQTT, FLAGS_NONE, event_reg)

DefPar_Fun(StatusBit, 28, 0, 0xFF, 0, U16_, Par_R, Par_Public, FLAGS_NONE, statusbits_reg)


DefPar_Fun(CasRozedneni, 48, 0, 0, 6, STRING_, Par_R, Par_Public, FLAGS_NONE, time_reg)
DefPar_Fun(CasZatmeni, 51, 0, 0, 6, STRING_, Par_R, Par_Public, FLAGS_NONE, time_reg)

DefPar_Fun(CasVychodu, 54, 0, 0, 6, STRING_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE, time_reg)
DefPar_Fun(CasZapadu, 57, 0, 0, 6, STRING_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE, time_reg)



/*
-----------------------------------------------------------------------------------------------------------
  @ Konfigurace

-----------------------------------------------------------------------------------------------------------
*/
    
DefPar_Nv(PIDRychlostP, 30, 50, 0, 10000, S16_, Par_RW, Par_Installer, FLAGS_NONE)
DefPar_Nv(PIDRychlostI, 31, 25, 0, 10000, S16_, Par_RW, Par_Installer, FLAGS_NONE)
DefPar_Nv(PIDPolohaP, 32, 1500, 0, 50000, S16_, Par_RW, Par_Installer, FLAGS_NONE)
DefPar_Nv(MaxRychlost, 33, 70, 30, 300, U16_, Par_RW, Par_Installer, FLAGS_NONE)
DefPar_Nv(MinRychlost, 34, 20, 0, 50, U16_, Par_RW, Par_Installer, FLAGS_NONE)

DefPar_Nv(LogLvl, 35, v_info, v_empty, v_info, U16_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(MotorProudOtMax, 36, 250, 10, 1000, S16_, Par_RW, Par_Installer, FLAGS_NONE)
DefPar_Nv(MotorProudOtMin, 37, 120, 5, 1000, S16_, Par_RW, Par_Installer, FLAGS_NONE)

DefPar_Nv(OsvetleniNoc, 38, 4, 0, 100, U16_, Par_RW, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Nv(OsvetleniDen, 39, 20, 0, 100, U16_, Par_RW, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Nv(ZpozdeniOtevreni, 40, 60, 0, 180, U16_, Par_RW, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Nv(ZpozdeniZavreni, 41, 60, 0, 180, U16_, Par_RW, Par_Public | Par_MQTT, FLAGS_NONE)

DefPar_Nv(OtevreniSvetlem, 42, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public | Par_MQTT, BOOL_FLAG)
DefPar_Nv(OtevreniVychodem, 44, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public | Par_MQTT, BOOL_FLAG)

DefPar_Nv(ZavreniSvetlem, 45, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public | Par_MQTT, BOOL_FLAG)
DefPar_Nv(ZavreniZapadem, 47, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public | Par_MQTT, BOOL_FLAG)

DefPar_Nv(CasDetekceSvetla, 60, 300, 0, 7200, U16_, Par_RW, Par_Public, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @ WiFi pripojeni

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Nv(WiFiSTAOn, 98, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public, BOOL_FLAG)
DefPar_Ram(WiFiAPOn, 99, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public, BOOL_FLAG)
DefPar_Nv(WiFihostname, 100, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(WiFiSSID, 116, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(WiFiHeslo, 132, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(StaticIP, 148, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public, BOOL_FLAG)
DefPar_Fun(LocalIP, 149, 0, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Fun(Gateway, 157, 0, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Fun(Mask, 165, -256, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Fun(DNS1, 173, 0, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Fun(DNS2, 181, 0, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Ram(MacAdresa, 189, 0, 0, 18, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiRSSI, 198, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiStatus, 199, wifi_vypnuto, wifi_vypnuto, wifi_hledani, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiAPClients, 200, 0, 0, 10, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Nv(CaptivePortal, 201, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public, BOOL_FLAG)

/*
-----------------------------------------------------------------------------------------------------------
  @ WiFi Scan

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Ram(WiFiScanCmd, 202, 0, 0, 1, U16_, Par_RW, Par_Public, BOOL_FLAG)

DefPar_Ram(WiFiScanSSID1, 203, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID2, 218, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID3, 233, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID4, 248, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID5, 263, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID6, 278, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanSSID7, 293, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI1, 308, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI2, 309, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI3, 310, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI4, 311, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI5, 312, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI6, 313, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI7, 314, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(WiFiScanRSSI8, 315, 0, -100, 0, S16_, Par_R, Par_Public, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @ Datum a cas

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Nv(NTPServer, 400, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(CasovePasmo, 416, Cesko, Cesko, Nezname_Pasmo, U16_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(PopisCasu, 417, 0, 0, 46, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Ram(ZemepisnaSirka, 440, 49, -90, 90, S16_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Ram(ZemepisnaDelka, 441, 16, -180, 180, S16_, Par_RW, Par_Public, FLAGS_NONE)

DefPar_Fun(AstroVychod, 442, 0, 0, 6, STRING_, Par_R, Par_Public, FLAGS_NONE, time_reg)
DefPar_Fun(AstroZapad, 445, 0, 0, 6, STRING_, Par_R, Par_Public, FLAGS_NONE, time_reg)
DefPar_Ram(NTPSyncCas, 448, 0, 0, 20, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(AktualniCas, 458, 0, 0, 20, STRING_, Par_R, Par_Public, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @ Zaznamy

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Fun(LogHistory, 512, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R, Par_Public, FLAGS_NONE, log_reg)
DefPar_Ext(LogHistory, 513, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 514, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 515, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 516, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 517, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 518, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 519, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 520, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 521, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 522, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 523, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 524, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 525, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 526, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)
DefPar_Ext(LogHistory, 527, NeniChyba, NeniChyba, MAX_ERROR, U16_, Par_R)

DefPar_Fun(Teplota24H, 600, 0, -100, 100, S16_, Par_R, Par_Public, FLAGS_NONE, chart_reg<int16_t>)
DefPar_Fun(Svetlo24H, 601, 0, 0, 100, U16_, Par_R, Par_Public, FLAGS_NONE, chart_reg<uint16_t>)
DefPar_Nv(TeplotaKorekce, 602, -6, -100, 100, S16_, Par_RW, Par_Public, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @  Zabezpeceni

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Fun(VstupPin, 700, -1, -1, 9999, S16_, Par_W, Par_Public, FLAGS_NONE, pwd_reg)
DefPar_Ram(AccessLvl, 701, Uzivatel, Uzivatel, Servis, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Nv(PinServis, 702, 1234, 0, 9999, U16_, Par_W, Par_Installer, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @ MQTT

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Fun(BrokerAdresa, 800, 0, 0, 16, STRING_, Par_RW, Par_Public, FLAGS_NONE, ipv4_reg_nv)
DefPar_Nv(BrokerPort, 808, 1883, 0, UINT16_MAX, U16_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(MQTTJmeno, 809, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(MQTTHeslo, 825, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Ram(MQTTInterval_S, 841, 10, 2, 120, U16_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(MQTTbase, 842, 0, 0, 32, STRING_, Par_RW, Par_Public, FLAGS_NONE)
DefPar_Nv(MQTTOn, 858, vypnuto, vypnuto, povoleno, U16_, Par_RW, Par_Public, BOOL_FLAG)
DefPar_Ram(MQTTStatus, 859, mqtt_vypnuto, mqtt_vypnuto, mqtt_odpojeno, S16_, Par_R, Par_Public, FLAGS_NONE)

/*
-----------------------------------------------------------------------------------------------------------
  @ System info

-----------------------------------------------------------------------------------------------------------
*/

DefPar_Ram(MainRev, 1000, MAIN_REVISION, MAIN_REVISION, MAIN_REVISION, U16_, Par_R, Par_Public | Par_MQTT, FLAGS_NONE)
DefPar_Ram(CompDate, 1001, 0, 0, 30, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(ResetReason, 1016, rst_Poweron, rst_Poweron, rst_External, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(SDKVersion, 1017, 0, 0, 20, STRING_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(HeapSize, 1027, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(HeapUsed, 1028, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(FlashSize, 1029, 0, 0, 16000, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(FlashSpeed, 1030, 0, 0, 80, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(CPUFreq, 1031, 0, 0, 240, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(PsramSize, 1032, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(PsramUsed, 1033, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(APPSize, 1034, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(APPUsed, 1035, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(FSSize, 1036, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(FSUsed, 1037, 0, 0, 300, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(Uptime, 1038, 0, 0, 300, S32_, Par_R, Par_Public, FLAGS_NONE)



/*
-----------------------------------------------------------------------------------------------------------
  @ Debug info

-----------------------------------------------------------------------------------------------------------
*/
DefPar_Ram(CRCError, 2000, 0, 0, 1000, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(TimeoutError, 2001, 0, 0, 1000, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(ExceptionError, 2002, 0, 0, 1000, U16_, Par_R, Par_Public, FLAGS_NONE)
DefPar_Ram(CorrectPackets, 2003, 0, 0, 1000, U16_, Par_R, Par_Public, FLAGS_NONE)

#endif /*PAR_DEF_INCLUDES*/

