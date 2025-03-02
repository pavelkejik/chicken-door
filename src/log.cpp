/***********************************************************************
 * Filename: log.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the SystemLog class, providing functions for 
 *     handling system logs. The log queue is managed using FreeRTOS to 
 *     facilitate efficient log handling, while the log entries are 
 *     stored in text files using LittleFS.
 *
 ***********************************************************************/


#include "log.h"
#include "common.h"
#include <LittleFS.h>
#include "freertos/queue.h"

uint8_t SystemLog::write_file;
QueueHandle_t SystemLog::log_queue;

const char *const SystemLog::log_files[] = {
    "/log_a.txt",
    "/log_b.txt",
};

void SystemLog::printItem(const Log_t *item)
{
    const char *verb_text[] = {"", "[ERROR]", "[WARN]", "[INFO]"};
    char buffer[30];
    struct tm tm_info = GetTime(item->time);
    strftime(buffer, 30, "%d.%m.%Y %H:%M:%S", &tm_info);
    Serial.printf("%s %s %s\n", verb_text[item->lvl], buffer, item->log_txt);
}

void SystemLog::PutLog(const char *msg, Verbosity_t lvl, time_t t)
{
    Log_t log_item;
    log_item.lvl = lvl;
    log_item.time = (t != 0) ? t : Now();
    strncpy(log_item.log_txt, msg, sizeof(log_item.log_txt) - 1);
    log_item.log_txt[sizeof(log_item.log_txt) - 1] = 0;

    printItem(&log_item);

    xQueueSend(log_queue, &log_item, (TickType_t)0);
}

void SystemLog::PutLog(String msg, Verbosity_t lvl, time_t t)
{
    PutLog(msg.c_str(), lvl, t);
}

void SystemLog::Init(void)
{
    log_queue = xQueueCreate(5, sizeof(Log_t));

    size_t file_items = 0;
    std::lock_guard<std::mutex> lock(storageFS_lock);
    File file = storageFS.open(log_files[0], "r");
    write_file = 0;
    if (file)
    {
        file_items = file.size() / sizeof(Log_t);
        if (file_items >= NMR_RECORDS)
        {
            write_file = 1;
        }
        file.close();
    }

    file = storageFS.open(log_files[1], "r");
    if (file)
    {
        file_items = file.size() / sizeof(Log_t);

        if (file_items >= NMR_RECORDS)
        {
            write_file = 0;
        }
        file.close();
    }
}

void SystemLog::Task(void)
{
    Log_t log_item;

    if (xQueueReceive(log_queue, &(log_item), (TickType_t)pdMS_TO_TICKS(10000)) == pdTRUE)
    {
        std::lock_guard<std::mutex> lock(storageFS_lock);

        File file = storageFS.open(log_files[write_file], "a");
        if (!file)
        {
            file = storageFS.open(log_files[write_file], "w", true);
        }
        if (file)
        {
            size_t file_items = file.size() / sizeof(Log_t);
            if (file_items > NMR_RECORDS)
            {
                file.close();
                write_file ^= 1;
                file = storageFS.open(log_files[write_file], "w", true);
            }
            file.write((uint8_t *)&log_item, sizeof(Log_t));
            file.close();
        }
    }
}


size_t SystemLog::GetLogJson(JsonArray doc, size_t pos, size_t nmr_max) {
    std::lock_guard<std::mutex> lock(storageFS_lock);

    size_t total_nmr = 0;

    int current_file_index = write_file;

    for (int i = 0; i < 2; ++i) {
        if (i == 1) {
            current_file_index ^= 1;
        }

        File file = storageFS.open(log_files[current_file_index], "r");
        if (!file) {
            continue;
        }

        size_t file_items = file.size() / sizeof(Log_t);
        total_nmr += file_items;

        size_t startRecord = (pos < file_items) ? file_items - pos : 0;
        size_t recordsToRead = min(nmr_max, startRecord);

        for (size_t recordIndex = startRecord; recordIndex > startRecord - recordsToRead; --recordIndex) {
            size_t position = (recordIndex - 1) * sizeof(Log_t);
            if (!file.seek(position)) {
                break;
            }

            Log_t log_item;
            size_t nmr_bytes = file.readBytes((char *)&log_item, sizeof(Log_t));
            if (nmr_bytes != sizeof(Log_t)) {
                break;
            }

            JsonDocument obj;
            obj["v"] = log_item.lvl;
            obj["t"] = log_item.time;
            obj["msg"] = log_item.log_txt;
            doc.add(obj);

            if (nmr_max > 0) {
                --nmr_max;
            }
        }

        pos = (pos >= file_items) ? pos - file_items : 0;

        file.close();
    }

    return total_nmr; // Return the total number of logs processed
}
