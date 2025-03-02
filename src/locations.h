/***********************************************************************
 * Filename: locations.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the Location class, which provides methods for 
 *     managing geographic locations and astronomical computations.
 *
 ***********************************************************************/


#pragma once
#include "Arduino.h"
#include "parameters.h"

#define TIME_TASKS_PERIOD_MS 1000

typedef enum
{
    astro_not_used = 0,
    astro_used = 1,
    astro_active = 2,
} AstroCtrl_t;

class Location
{
private:
    static const char *const tz_strings[];
    static const int16_t latitudes[];
    static const int16_t longitudes[];

    static Locations_t act_loc;
    static bool validTime;
    static bool isDay;

public:
    static time_t lastTimeSync;
    static void Init(void);

    static void CheckLocalization(void);
    static bool GetSunriseSunset(time_t now, time_t &previous_time, time_t &next_time);
    static void ComputeAstro(void);
    static AstroCtrl_t IsAstroDayCmd(void);
    static AstroCtrl_t IsAstroNightCmd(void);
};