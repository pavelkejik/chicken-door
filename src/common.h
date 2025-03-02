/***********************************************************************
 * Filename: common.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Provides various utility functions and definitions for handling 
 *     time, memory allocation, file I/O.
 *  
 ***********************************************************************/

#pragma once

#include "Arduino.h"
#include <LittleFS.h>
#include <mutex>
#include "ArduinoJson.h"

#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__
#define COMPILE_DATE_TIME (__DATE__ " " __TIME__)

#define COMMON_LOOP_TASK_PERIOD_MS 30

struct SpiRamAllocator : ArduinoJson::Allocator
{
    void *allocate(size_t size) override
    {
        return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    }

    void deallocate(void *pointer) override
    {
        heap_caps_free(pointer);
    }

    void *reallocate(void *ptr, size_t new_size) override
    {
        return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
    }
};


void SetTimezone(const char *tz);

time_t Now(void);

struct tm GetTime(time_t t);

struct tm GetTime(void);

int Year(void);

int Month(void);

int Day(void);

int Hour(void);

int Minute(void);

int Second(void);

bool IsDST(void);

int Weekday(void);

void SetDateTime(time_t t);

time_t MakeEpochTime(int year, int month, int day, int hour, int min);

void SetDateTime(int year, int month, int day, int hours, int minutes);

bool EndTimer(uint32_t& timer);

void StopTimer(uint32_t& timer);

void StartTimer(uint32_t& timer, uint32_t value);

bool IsTimerRunning(const uint32_t& timer);

bool LoadJsonFile(const String filename, JsonDocument &doc);

void SaveJsonFile(const String filename, JsonDocument &doc);

void StringToMac(const String& macString, uint8_t* macAddress);

String MacToString(const uint8_t *macAddress);

extern SpiRamAllocator allocator;

extern fs::LittleFSFS storageFS;

extern std::mutex storageFS_lock;

