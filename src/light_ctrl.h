/***********************************************************************
 * Filename: light_ctrl.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the LightCtrl class for managing light-based control 
 *     logic. It utilizes filters and debounce logic to handle sensor
 *     data and defines various constants related to light detection and timing.
 *
 ***********************************************************************/


#include "Arduino.h"
#include "exp_filter.h"
#include "debounce.h"
#include "parameters.h"

#ifndef LIGHTCTRL_H_
#define LIGHTCTRL_H_

#define AUTO_DETECTION_TASK_PERIOD_S 2
#define LIGHT_LVL_DETECTION_TIME_S (5 * 60)

#define LIGHT_DET_CONST(_t) ((_t) / AUTO_DETECTION_TASK_PERIOD_S)

#define CHANGE_TIME_MIN 5
#define MIN_STEADY_TIME_MIN (5 * 60)
#define Vphotodark 2800.0
#define Vphotolight 30.0

typedef enum
{
	NoCmd,
	OpenCmd,
	CloseCmd
} lightCmd_t;

class LightCtrl
{
private:
	ExpFilter photo_mv;
	Debounce open_delay;
	Debounce close_delay;
	Debounce sunrise_det;
	Debounce sunset_det;

	uint8_t timeToDay;
	uint8_t timeToNight;
	int32_t lastligthAD;
	void LightConv(uint16_t adx);
	DoorCmdSource_t cmdSource;

public:
	LightCtrl() : photo_mv(16),
				  open_delay(1, 1),
				  close_delay(1, 1),
				  sunrise_det(1, 2),
				  sunset_det(1, 2){};

	lightCmd_t LastCmd;
	void Init(void);
	void LightUpdate(void);
	bool IsNight(void);
	bool DetectLightNight(void);
	bool IsDay(void);
	bool DetectLightDay(void);
	void Task(void);
	bool IsLight(void);
	bool IsDark(void);
};

#endif /* LIGHTCTRL_H_ */
