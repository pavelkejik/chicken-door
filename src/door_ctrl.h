/***********************************************************************
 * Filename: door_ctrl.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the DoorCtrl class, which serves as the core of the 
 *     finite state machine for door control.
 *
 ***********************************************************************/


#pragma once

#include "common.h"
#include "parameters.h"
#include "FreeRTOS.h"
#include <mutex>
#include "debounce.h"

#define CONTROL_TASK_PERIOD_MS 100

#define MIN_OPEN_INTERVAL_PLS 125
#define MAX_OPEN_INTERVAL_PLS 180
#define MAX_INIT_OPEN_INTERVAL_PULS MAX_OPEN_INTERVAL_PLS
#define TOL_CLOSE_POS_PLS 5
#define TOL_OPEN_POS_PLS 5
#define RELEASE_POS 20
#define STEADY_MOVE_TOL 5

#define NOSTATE nmr_states

typedef enum
{
    ev_stop = 0,
    ev_open = 1,
    ev_close = 2,
    ev_target = 3,
    ev_low_limit = 4,
    ev_high_limit = 5,
    ev_overload_up = 6,
    ev_overload_down = 7,
    ev_no_move = 8,
    ev_timer_expired = 9,
    nmr_events = 10,
    no_event = 11,
    first_event = 0
} DoorEvents_t;

typedef DoorState_t (*state_fun_t)(void);

class DoorCtrl
{
private:
    static std::mutex events_mutex;
    static Debounce steady_move;
    static std::atomic<uint32_t> events;
    static const state_fun_t state_fun[nmr_states][nmr_events];
    static uint32_t timer;
    static void execute(void);

public:
    static void Init(void);
    static void Task(void)
    {
        int32_t open_pos = Otevreno_puls.Get();
        int32_t act_pos = AktualniPoloha_puls.Get();

        if ((HorniPoloha.Get() == Overen) && (DolniPoloha.Get() == Overen))
        {
            Otevreni_proc.SetLimit(act_pos * 100 / open_pos);
        }
        else if (StavDvirka.Get() == Zavreno)
        {
            Otevreni_proc.Set(0);
        }
        else if (StavDvirka.Get() == Otevreno)
        {
            Otevreni_proc.Set(100);
        }
        execute();
    }
    static void Event(DoorEvents_t event)
    {
        events |= 1 << event;
    }

    static void EventSourced(DoorEvents_t event, DoorCmdSource_t source)
    {
        events |= 1 << event;
        PovelOd.Set(source);
    }

    static bool IsState(DoorState_t state);
    static void SetState(DoorState_t state);
    static void ChangePos(int16_t delta_pos);
    static bool PosInRange(int16_t min_pos, int16_t max_pos);
    static void TimerStart(uint32_t secs);
};