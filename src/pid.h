/***********************************************************************
 * Filename: pid.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the PID class for proportional-integral (PI) control.
 *
 ***********************************************************************/

#pragma once
#include "stdint.h"
#include "stdarg.h"

#define PISCALLER 1000

class PID
{
private:
uint16_t  Pconst;
uint16_t  Iconst;
int16_t FeedForwardMin;
int16_t FeedForwardMax;
int32_t  Suma;
int32_t Minout;
int32_t Maxout;
public:
	PID();
	void Outrange(int32_t Min,int32_t Max, int32_t FFMin = 0, int32_t FFMax = 0);
	void Setparam(uint16_t P,uint16_t I);
	int32_t Evaluate(int32_t Err);
	int32_t Evaluate(int32_t Err,int32_t min,int32_t max);
	int32_t Setout(int32_t Out, int32_t Err=0);
};