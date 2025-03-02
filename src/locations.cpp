/***********************************************************************
 * Filename: locations.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the Location class, manages location-based
 *     settings like time zones and coordinates, and includes
 *     logic to determine whether it's day or night based
 *     on sunrise/sunset calculations.
 *
 ***********************************************************************/


#include "locations.h"
#include <SunRise.h>
#include "light_ctrl.h"

// Cesko = 0,
// Slovensko = 1,
// Polsko = 2,
// Nemecko = 3,
// Rakousko = 4,

const char *const Location::tz_strings[] = {
    "CET-1CEST,M3.5.0,M10.5.0/3",
    "CET-1CEST,M3.5.0,M10.5.0/3",
    "CET-1CEST,M3.5.0,M10.5.0/3",
    "CET-1CEST,M3.5.0,M10.5.0/3",
    "CET-1CEST,M3.5.0,M10.5.0/3"};

const int16_t Location::latitudes[] = {
    49,
    47,
    52,
    51,
    48};

const int16_t Location::longitudes[] = {
    16,
    20,
    19,
    10,
    14};

time_t Location::lastTimeSync;

Locations_t Location::act_loc;
bool Location::validTime;
bool Location::isDay;

void Location::Init(void)
{
    validTime = false;
    lastTimeSync = -100;
    act_loc = (Locations_t)CasovePasmo.Get();
    if (act_loc >= CasovePasmo.getMax())
    {
        act_loc = (Locations_t)CasovePasmo.getDefault();
        CasovePasmo.Set(act_loc);
        ZemepisnaSirka.Set((int32_t)latitudes[act_loc]);
        ZemepisnaDelka.Set((int32_t)longitudes[act_loc]);
        PopisCasu.Set(tz_strings[act_loc]);
    }

    if (PopisCasu.Get().isEmpty())
    {
        PopisCasu.Set(tz_strings[act_loc]);
    }

    SetTimezone(PopisCasu.Get().c_str());
}

void Location::CheckLocalization(void)
{
    static int lastCalcDay = 0;
    static int lastCalcYear = 0;
    static time_t lastSunsetTime = 0;

    if (act_loc != (Locations_t)CasovePasmo.Get())
    {
        act_loc = (Locations_t)CasovePasmo.Get();
        if (act_loc >= CasovePasmo.getMax())
        {
            act_loc = (Locations_t)CasovePasmo.getDefault();
            CasovePasmo.Set(act_loc);
        }

        ZemepisnaSirka.Set((int32_t)latitudes[act_loc]);
        ZemepisnaDelka.Set((int32_t)longitudes[act_loc]);
        PopisCasu.Set(tz_strings[act_loc]);

        SetTimezone(PopisCasu.Get().c_str());
        lastSunsetTime = -1;
    }

    time_t actTime = Now();
    if (actTime >= lastTimeSync && actTime <= (lastTimeSync + 10 * AUTO_DETECTION_TASK_PERIOD_S))
    {
        if (!validTime)
        {
            lastSunsetTime = -1;
            validTime = true;
        }
        lastTimeSync = actTime;
        if (isDay)
        {
            if ((actTime > AstroZapad.Get()) && (AstroZapad.Get() != 0))
            {
                isDay = false;
            }
        }
        else
        {
            if ((actTime > AstroVychod.Get()) && (AstroVychod.Get() != 0))
            {
                isDay = true;
            }
        }
    }
    else
    {
        validTime = false;
    }

    if (validTime && actTime > (lastSunsetTime + (5 * 60)))
    {
        ComputeAstro();
        lastSunsetTime = actTime;
    }

    tm tm_info = GetTime(actTime);
    char formattedTime[20];
    strftime(formattedTime, 20, "%d.%m.%y %H:%M", &tm_info);
    AktualniCas.Set(formattedTime);
}

void Location::ComputeAstro(void)
{
    time_t prev;
    time_t next;
    isDay = GetSunriseSunset(Now(), prev, next);

    if (isDay)
    {
        AstroVychod.Set(prev);
        AstroZapad.Set(next);
    }
    else
    {
        AstroVychod.Set(next);
        AstroZapad.Set(prev);
    }
}

AstroCtrl_t Location::IsAstroDayCmd(void)
{
    if (validTime && OtevreniVychodem.Get())
    {
        if (isDay)
        {
            return astro_active;
        }
        return astro_used;
    }
    return astro_not_used;
}

AstroCtrl_t Location::IsAstroNightCmd(void)
{
    if (validTime && ZavreniZapadem.Get())
    {
        if (!isDay)
        {
            return astro_active;
        }
        return astro_used;
    }
    return astro_not_used;
}

// return true if sun is visible at *time*.
bool Location::GetSunriseSunset(time_t now, time_t &previous_time, time_t &next_time)
{
    SunRise sr;
    sr.calculate((double)ZemepisnaSirka.Get(), (double)ZemepisnaDelka.Get(), now);

    // Preceding event
    if ((!sr.hasRise || (sr.hasRise && sr.riseTime > sr.queryTime)) &&
        (!sr.hasSet || (sr.hasSet && sr.setTime > sr.queryTime)))
    {
        // No sun rise or set during preceding hours
        previous_time = 0;
    }
    else if (sr.hasRise && sr.riseTime < sr.queryTime)
    {
        // Sun rise
        previous_time = sr.riseTime;
    }
    else if (sr.hasSet && sr.setTime < sr.queryTime)
    {
        // Sun set
        previous_time = sr.setTime;
    }

    // Succeeding event
    if ((!sr.hasRise || (sr.hasRise && sr.riseTime < sr.queryTime)) &&
        (!sr.hasSet || (sr.hasSet && sr.setTime < sr.queryTime)))
    {
        // No sun rise or set during succeeding hours
        next_time = 0;
    }
    else if (sr.hasRise && sr.riseTime > sr.queryTime)
    {
        // Sun rise
        next_time = sr.riseTime;
    }
    else if (sr.hasSet && sr.setTime > sr.queryTime)
    {
        // Sun set
        next_time = sr.setTime;
    }

    return sr.isVisible;
}