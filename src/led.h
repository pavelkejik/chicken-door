/***********************************************************************
 * Filename: led.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the Led class to manage LED control patterns and 
 *     operations. It includes methods for initialization, setting 
 *     control patterns, and executing control tasks.
 *
 ***********************************************************************/


#pragma once

#include "Arduino.h"

#define LED_CTRL_TIME_MS 70
class Led
{
private:
	uint16_t CtrlPattern;
	uint8_t CtrlStep;
	uint8_t pinNmr;

public:
	Led(uint8_t pin) : CtrlPattern(0), pinNmr(pin)
	{
	}
	void Init(void)
	{
		pinMode(pinNmr, OUTPUT);
	}
	void SetCtrl(uint16_t Pattern)
	{
		CtrlPattern = Pattern;
	}
	void CtrlTask(void)
	{
		if (++CtrlStep >= 16)
		{
			CtrlStep = 0;
		}
		digitalWrite(pinNmr, ((CtrlPattern & (1 << CtrlStep)) != 0) ? HIGH : LOW);
	}
	Led &operator=(uint16_t in)
	{
		SetCtrl(in);
		return *this;
	}
};
extern Led LedR, LedG;