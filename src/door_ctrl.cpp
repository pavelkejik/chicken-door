/***********************************************************************
 * Filename: door_ctrl.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Defines event processing functions for the finite state machine 
 *     in the DoorCtrl class.
 *
 ***********************************************************************/


#include <door_ctrl.h>
#include "led.h"
#include "servo.h"
#include "log.h"

#define TIME_SCHEDULE(_t_secs) ((uint32_t)((_t_secs) * 1000 / CONTROL_TASK_PERIOD_MS))

std::atomic<uint32_t> DoorCtrl::events;
uint32_t DoorCtrl::timer;
std::mutex DoorCtrl::events_mutex;
Debounce DoorCtrl::steady_move(5, 20);

extern Encoder encoder;
extern Servo servo;

const char *const cmd_source_txt[] = {
    "",
    "(Tlacitkem)",
    "(Osvetlenim)",
    "(Casem)",
    "(Modbus)",
    "(HTTP)",
    ""};

DoorState_t noaction()
{
    return NOSTATE;
}

DoorState_t init_open()
{
    SystemLog::PutLog("Start otevirani na doraz " + String(cmd_source_txt[PovelOd.Get()]), v_info);
    servo.ChangePosition(MAX_INIT_OPEN_INTERVAL_PULS);
    return OteviraniMax;
}

DoorState_t init_close()
{
    SystemLog::PutLog("Start zavirani na doraz " + String(cmd_source_txt[PovelOd.Get()]), v_info);
    servo.ChangePosition(-MAX_INIT_OPEN_INTERVAL_PULS);
    return ZaviraniMin;
}

DoorState_t closed()
{
    SystemLog::PutLog("Zavreno", v_info);
    servo.Stop();
    error_close.Clear();
    CasZavreni.Set(Now());
    return Zavreno;
}

DoorState_t opened()
{
    SystemLog::PutLog("Otevreno", v_info);
    servo.Stop();
    error_open.Clear();
    CasOtevreni.Set(Now());
    return Otevreno;
}

DoorState_t stop()
{
    SystemLog::PutLog("Stop", v_info);

    servo.Stop();

    if (DolniPoloha.Get() == Overen)
    {
        if (DoorCtrl::PosInRange(-TOL_CLOSE_POS_PLS, TOL_CLOSE_POS_PLS))
        {
            return closed();
        }
        else if (HorniPoloha.Get() == Overen)
        {
            if (DoorCtrl::PosInRange(Otevreno_puls.Get() - 2 * TOL_OPEN_POS_PLS, Otevreno_puls.Get() + TOL_OPEN_POS_PLS))
            {
                return opened();
            }
            else
            {

                return MeziPoloha;
            }
        }
    }

    return NeznaznamaPoloha;
}

DoorState_t break_close()
{
    // SystemLog::PutLog("Stop", v_info);

    servo.Stop();

    DoorCtrl::TimerStart(3);

    return StopZavirani;
}

DoorState_t break_open()
{
    // SystemLog::PutLog("Stop", v_info);

    servo.Stop();

    DoorCtrl::TimerStart(3);

    return StopOtevirani;
}

DoorState_t detect_open()
{
    uint16_t state = HorniPoloha.Get() + 1;
    Otevreno_puls.SetLimit(AktualniPoloha_puls.Get());

    servo.Stop();

    if (state < Overen)
    {
        if ((DolniPoloha.Get() == Overen) && DoorCtrl::PosInRange(MIN_OPEN_INTERVAL_PLS, MAX_OPEN_INTERVAL_PLS))
        {
            HorniPoloha.Set(Overen);
            return opened();
        }

        SystemLog::PutLog("Odjezd od horniho dorazu", v_info);

        HorniPoloha.Set(state);

        servo.ChangePosition(-RELEASE_POS);

        return UvolneniOtevirani;
    }
    else
    {
        HorniPoloha.Set(Overen);
        return opened();
    }
}

DoorState_t repeat_open()
{
    SystemLog::PutLog("Opakovani otevreni", v_info);
    servo.ChangePosition(MAX_INIT_OPEN_INTERVAL_PULS);
    return OteviraniMax;
}

DoorState_t detect_close()
{
    uint16_t state = DolniPoloha.Get() + 1;
    servo.Stop();
    if (state < Overen)
    {
        if ((HorniPoloha.Get() == Overen) && DoorCtrl::PosInRange((int16_t)Otevreno_puls.Get() - MAX_OPEN_INTERVAL_PLS, (int16_t)Otevreno_puls.Get() - MIN_OPEN_INTERVAL_PLS))
        {
            Otevreno_puls.SetLimit(Otevreno_puls.Get() - AktualniPoloha_puls.Get());
            encoder.SetPosition(0);
            DolniPoloha.Set(Overen);
            return closed();
        }

        SystemLog::PutLog("Odjezd od dolniho dorazu", v_info);

        DolniPoloha.Set(state);
        servo.ChangePosition(RELEASE_POS);

        return UvolneniZavirani;
    }
    else
    {
        DolniPoloha.Set(Overen);

        if (HorniPoloha.Get() == Overen)
        {
            Otevreno_puls.SetLimit(Otevreno_puls.Get() - AktualniPoloha_puls.Get());
        }

        encoder.SetPosition(0);

        return closed();
    }
}

DoorState_t repeat_close()
{
    SystemLog::PutLog("Opakovani zavreni", v_info);
    servo.ChangePosition(-MAX_INIT_OPEN_INTERVAL_PULS);
    return ZaviraniMin;
}

DoorState_t unable_open()
{
    servo.Stop();

    if (DoorCtrl::PosInRange(Otevreno_puls.Get() - 2 * TOL_OPEN_POS_PLS, Otevreno_puls.Get() + 2 * TOL_OPEN_POS_PLS))
    {
        return opened();
    }

    if (error_open.Check(true))
    {
        HorniPoloha.Set(Nedetekovan);
        DolniPoloha.Set(Nedetekovan);
        DoorCtrl::TimerStart(2);
        return ChybaDveri;
    }

    // servo.ChangePosition(-RELEASE_POS);

    DoorCtrl::TimerStart(3);

    return PreruseniOtevirani;
}

DoorState_t relay_off()
{
    servo.MotorPWR(false);
    return ChybaDveri;
}

DoorState_t relay_on()
{
    servo.MotorPWR(true);
    return stop();
}

DoorState_t to_close()
{
    if (DolniPoloha.Get() != Overen)
    {
        return init_close();
    }

    SystemLog::PutLog("Zavirani " + String(cmd_source_txt[PovelOd.Get()]), v_info);

    servo.SetPosition(-TOL_CLOSE_POS_PLS);
    return Zavirani;
}

DoorState_t close_to_min()
{
    if (AktualniPoloha_puls.Get() < -MAX_INIT_OPEN_INTERVAL_PULS)
    {
        SystemLog::PutLog("Nenalezen dolni doraz", v_error);
        servo.Stop();
        HorniPoloha.Set(Nedetekovan);
        DolniPoloha.Set(Nedetekovan);
        DoorCtrl::TimerStart(1);
        return ChybaDveri;
    }

    HorniPoloha.Set(Nedetekovan);
    DolniPoloha.Set(Nedetekovan);
    servo.ChangePosition(-RELEASE_POS);
    return NOSTATE;
}

DoorState_t unable_close()
{
    servo.Stop();

    if (error_close.Check(true))
    {
        HorniPoloha.Set(Nedetekovan);
        DolniPoloha.Set(Nedetekovan);
        DoorCtrl::TimerStart(1);
        return ChybaDveri;
    }

    // servo.ChangePosition(RELEASE_POS);

    DoorCtrl::TimerStart(3);

    return PreruseniZavirani;
}

DoorState_t repeat_to_min()
{
    servo.Stop();
    if (DoorCtrl::PosInRange(-TOL_CLOSE_POS_PLS, TOL_CLOSE_POS_PLS))
    {
        return closed();
    }

    encoder.SetPosition(0);
    servo.Stop();
    DolniPoloha.Set(Overen);
    HorniPoloha.Set(Nedetekovan);
    CasZavreni.Set(Now());
    return Zavreno;
}

DoorState_t to_open()
{
    if (HorniPoloha.Get() != Overen)
    {
        return init_open();
    }

    SystemLog::PutLog("Otevirani " + String(cmd_source_txt[PovelOd.Get()]), v_info);

    servo.SetPosition(Otevreno_puls.Get() + TOL_OPEN_POS_PLS);
    return Otevirani;
}

bool DoorCtrl::PosInRange(int16_t min_pos, int16_t max_pos)
{
    return AktualniPoloha_puls.Get() >= min_pos && AktualniPoloha_puls.Get() <= max_pos;
}

//  Stop            Otevrit         Zavrit         Cilova poloha    dolni doraz      horni doraz        pretizeni N         pretizeni D        bez pohybu     time expired
#undef State
#define State(_state_, _ev_stop, _ev_open, _ev_close, _ev_target, _ev_low_limit, _ev_high_limit, _ev_overload_up, _ev_overload_down, _ev_no_move, _ev_timer_expired) \
    {_ev_stop, _ev_open, _ev_close, _ev_target, _ev_low_limit, _ev_high_limit, _ev_overload_up, _ev_overload_down, _ev_no_move, _ev_timer_expired},

const state_fun_t DoorCtrl::state_fun[nmr_states][nmr_events] =
    {
#include "door_ctrl_table.h"
};

void DoorCtrl::execute()
{
    if (EndTimer(timer))
    {
        Event(ev_timer_expired);
    }

    switch (StavDvirka.Get())
    {
    case NeznaznamaPoloha:
    case Otevreno:
    case Zavreno:
    case MeziPoloha:
        if (steady_move.InpEvent((AktualniPoloha_puls.Get() > (PozadovanaPoloha_puls.Get() + STEADY_MOVE_TOL)) || (AktualniPoloha_puls.Get() < (PozadovanaPoloha_puls.Get() - STEADY_MOVE_TOL))))
        {
            if ((DolniPoloha.Get() != Nedetekovan) || (HorniPoloha.Get() != Nedetekovan))
            {
                SystemLog::PutLog("Neocekavany pohyb dveri: aktualni poloha neznama", v_warning);
                DolniPoloha.Set(Nedetekovan);
                HorniPoloha.Set(Nedetekovan);
                StavDvirka.Set(NeznaznamaPoloha);
            }
        }
        break;

    default:
        steady_move.Reset();
        break;
    }

    // std::lock_guard<std::mutex> lock(events_mutex);
    if (events != 0)
    {
        for (int i = first_event; i < nmr_events; i++)
        {
            uint32_t event_mask = 1 << i;
            if (events & event_mask)
            {
                DoorState_t state = state_fun[StavDvirka.Get()][i]();
                if (state != NOSTATE)
                {
                    SetState(state);
                }
                events &= ~event_mask;
            }
        }
    }
}

void DoorCtrl::Init(void)
{
}

bool DoorCtrl::IsState(DoorState_t state)
{
    return (DoorState_t)StavDvirka.Get() == state;
}
void DoorCtrl::SetState(DoorState_t state)
{
    StavDvirka.Set(state);
}

void DoorCtrl::TimerStart(uint32_t secs)
{
    StartTimer(timer, TIME_SCHEDULE(secs));
}