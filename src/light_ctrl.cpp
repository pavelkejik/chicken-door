/***********************************************************************
 * Filename: light_ctrl.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the LightCtrl class.
 *
 ***********************************************************************/



#include "light_ctrl.h"
#include "parameters.h"
#include "pin_map.h"
#include "door_ctrl.h"
#include "log.h"
#include "locations.h"

void LightCtrl::Init(void)
{
	timeToDay = 0;
	timeToNight = 0;
	LastCmd = NoCmd;

	open_delay.Reset();
	close_delay.Reset();

	open_delay.SetMax(ZpozdeniOtevreni.Get());
	close_delay.SetMax(ZpozdeniZavreni.Get());
	// Casovani = ZpozdeniZavreni;
}

void LightCtrl::LightConv(uint16_t mv)
{
	float temp = 100.0f * (mv - Vphotodark);
	temp = temp / (Vphotolight - Vphotodark);
	if (temp < 0)
	{
		temp = 0;
	}
	Osvetleni_proc.SetLimit((uint16_t)temp);
}

bool LightCtrl::DetectLightNight(void)
{
	if (sunset_det.InpEvent((Osvetleni_proc.Get() < OsvetleniNoc.Get())))
	{
		CasZatmeni.Set(Now());
	}
	return sunset_det.Out();
}

bool LightCtrl::DetectLightDay(void)
{
	if (sunrise_det.InpEvent((Osvetleni_proc.Get() > OsvetleniDen.Get())))
	{
		CasRozedneni.Set(Now());
	}
	return sunrise_det.Out();
}

bool LightCtrl::IsNight(void)
{
	switch (Location::IsAstroNightCmd())
	{
	case astro_active:
		cmdSource = povel_casem;
		CasZapadu.Set(AstroZapad.Get());
		AutomatikaZavreni.Set(Casem);
		return true;

	case astro_used:
		CasZapadu.Set(AstroZapad.Get());
		AutomatikaZavreni.Set(Casem);
		break;

	default:
		if (ZavreniSvetlem.Get())
		{
			AutomatikaZavreni.Set(Svetlem);
			CasZapadu.Set(CasZatmeni.Get());
			if (IsDark())
			{
				cmdSource = povel_osvetlenim;
				return true;
			}
		}
		else
		{
			AutomatikaZavreni.Set(Manualni);
			CasZapadu.Set(0);
		}
		break;
	}
	return false;
}

bool LightCtrl::IsDay(void)
{
	switch (Location::IsAstroDayCmd())
	{
	case astro_active:
		cmdSource = povel_casem;
		CasVychodu.Set(AstroVychod.Get());
		AutomatikaOtevreni.Set(Casem);
		return true;

	case astro_used:
		CasVychodu.Set(AstroVychod.Get());
		AutomatikaOtevreni.Set(Casem);
		break;

	default:
		if (OtevreniSvetlem.Get())
		{
			AutomatikaOtevreni.Set(Svetlem);
			CasVychodu.Set(CasRozedneni.Get());
			if (IsLight())
			{
				cmdSource = povel_osvetlenim;
				return true;
			}
		}
		else
		{
			AutomatikaOtevreni.Set(Manualni);
			CasVychodu.Set(0);
		}
		break;
	}
	return false;
}

bool LightCtrl::IsLight(void)
{
	return sunrise_det.Out();
}

bool LightCtrl::IsDark(void)
{
	return sunset_det.Out();
}

void LightCtrl::Task(void)
{
	LightConv(photo_mv.Input(analogReadMilliVolts(PHOTO_R)));
	open_delay.SetMax(LIGHT_DET_CONST(ZpozdeniOtevreni.Get() * 60));
	close_delay.SetMax(LIGHT_DET_CONST(ZpozdeniZavreni.Get() * 60));
	sunrise_det.SetMax(LIGHT_DET_CONST(CasDetekceSvetla.Get()));
	sunset_det.SetMax(LIGHT_DET_CONST(CasDetekceSvetla.Get()));

	DetectLightDay();
	DetectLightNight();

	bool isNight = IsNight();
	bool isDay = IsDay();
	if (isNight)
	{
		open_delay.Reset();
		CasDoZavreni.Set((close_delay.GetCnt() * AUTO_DETECTION_TASK_PERIOD_S + 59) / 60);
		CasDoOtevreni.Set(0);
		if (close_delay.InpEvent(true))
		{
			if (StavDvirka.Get() != Zavreno)
			{
				DoorCtrl::EventSourced(ev_close, cmdSource);
			}
		}
	}
	else if (isDay)
	{
		close_delay.Reset();
		CasDoZavreni.Set(0);
		CasDoOtevreni.Set((open_delay.GetCnt() * AUTO_DETECTION_TASK_PERIOD_S + 59) / 60);
		if (open_delay.InpEvent(true))
		{
			if (StavDvirka.Get() != Otevreno)
			{
				DoorCtrl::EventSourced(ev_open, cmdSource);
			}
		}
	}
	else
	{
		CasDoZavreni.Set(0);
		CasDoOtevreni.Set(0);
		open_delay.Inp(false);
		close_delay.Inp(false);
	}
}
