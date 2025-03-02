/***********************************************************************
 * Filename: pid.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the PID class for proportional-integral (PI) control
 *     handling from the pid.h header.
 *
 ***********************************************************************/

#include <pid.h>

PID::PID()
{
}

void PID::Setparam(uint16_t P, uint16_t I)
{
	Pconst = P;
	Iconst = I;
}

void PID::Outrange(int32_t Min, int32_t Max, int32_t FFMin, int32_t FFMax)
{
	Minout = Min;
	Maxout = Max;
	FeedForwardMin = FFMin < Min ? Min : FFMin;
	FeedForwardMax = FFMax > Max ? Max : FFMax;
}

int32_t PID::Evaluate(int32_t Err, int32_t min, int32_t max)
{
	Minout = min;
	Maxout = max;
	return Evaluate(Err);
}

int32_t PID::Evaluate(int32_t Err)
{

	int32_t out;
	/*P component*/
	out = Pconst * Err;
	/*I  component*/
	Suma = Suma + Err;
	out += Iconst * Suma;

	/*Wind-up*/
	int32_t temp = (int32_t)out / PISCALLER;
	if (temp < Minout)
	{
		temp = Minout;
		Setout(Minout, Err);
	}
	else if (temp > Maxout)
	{
		temp = Maxout;
		Setout(Maxout, Err);
	}

	if ((temp < FeedForwardMax) && (temp > FeedForwardMin))
	{
		if (Err > 0)
		{
			temp = FeedForwardMax;
			Setout(temp, Err);
		}
		else if (Err < 0)
		{
			temp = FeedForwardMin;
			Setout(temp, Err);
		}
	}

	return temp;
}

int32_t PID::Setout(int32_t out, int32_t Err)
{
	int32_t temp;

	if (Iconst > 0)
	{
		temp = Pconst * Err;
		temp = out * PISCALLER - temp;
		temp /= Iconst;
		Suma = (int32_t)temp;
	}

	return out;
}
