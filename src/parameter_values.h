/***********************************************************************
 * Filename: parameter_values.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Defines constants, enumerations, and flags for parameter values used
 *     throughout the application.
 *
 ***********************************************************************/

#pragma once

#define MAIN_REVISION 23

#define FLAGS_NONE 0
#define BOOL_FLAG 0x01
#define COMM_PERIOD_FLAG 0x02
#define CHART_FLAG 0x04
#define FW_VERSION_FLAG 0x08

#define ERR_HISTORY_CNT 16

typedef enum
{
	NeznaznamaPoloha = 0,
	Otevirani = 1,
	OteviraniMax = 2,
	Otevreno = 3,
	PreruseniOtevirani = 4,
	UvolneniOtevirani = 5,

	Zavirani = 6,
	ZaviraniMin = 7,
	Zavreno = 8,
	PreruseniZavirani = 9,
	UvolneniZavirani = 10,
	MeziPoloha = 11,

	StopZavirani = 12,
	StopOtevirani = 13,

	ChybaDveri = 14,
	nmr_states
} DoorState_t;

typedef enum
{
	SOtevirani = 1,
	SZavirani = 2,
	SOteviraniMax = 4,
	SZaviraniMin = 8,
	SOtevreno = 16,
	SZavreno = 32,
	SChybaDveri = 0x8000
} DoorStatusBits_t;

typedef enum
{
	Nedetekovan = 0,
	Detekovan = 1,
	Overen = 2,
} DoorLimit_t;

typedef enum
{
	Manualni = 0,
	Svetlem = 1,
	Casem = 2
} AutoOvladani_t;

typedef enum
{
	Uzivatel = 0,
	Servis = 1,
} ActiveAccess_t;

typedef enum
{
	NeniChyba = 0,
	NadProud1N = 1,
	NadProud2N = 2,
	NeniPohybN = 3,
	DorazH = 4,
	NadProud1D = 5,
	NadProud2D = 6,
	NeniPohyb = 7,
	DorazD = 8,
	ChybaZavirani = 9,
	ChybaOtevirani = 10,
	MAX_ERROR = 100
} ErrorState_t;

typedef enum
{
	v_empty = 0,
	v_error = 1,
	v_warning = 2,
	v_info = 3,
} Verbosity_t;

typedef enum
{
	vypnuto = 0,
	povoleno = 1,
} AutoControl_t;

// Odpovida DoorEvents_t
typedef enum
{
	manual_stop = 0,
	manual_otevrit = 1,
	manual_zavrit = 2,
} DoorEventsManual_t;

typedef enum
{
	povel_neni = 0,
	povel_tlacitkem = 1,
	povel_osvetlenim = 2,
	povel_casem = 3,
	povel_modbus = 4,
	povel_webem = 5,
	povel_neznamy = 6
} DoorCmdSource_t;

typedef enum
{
	wifi_vypnuto = 0,
	wifi_pripojovani = 1,
	wifi_pripojeno = 2,
	wifi_odpojeno = 3,
	wifi_hledani = 10,
} WiFiState_t;

typedef enum
{
	mqtt_vypnuto = 0,
	mqtt_pripojovani = 1,
	mqtt_pripojeno = 2,
	mqtt_odpojeno = 3,
} MQTTState_t;

typedef enum
{
	Cesko = 0,
	Slovensko = 1,
	Polsko = 2,
	Nemecko = 3,
	Rakousko = 4,
	Nezname_Pasmo = 10
} Locations_t;

typedef enum
{
	rst_Unknown = 0,
    rst_Software = 1,  
    rst_Watchdog = 2,
    rst_Brownout = 3,
	rst_Poweron = 4,      
    rst_External = 5,
} ResetReason_t;