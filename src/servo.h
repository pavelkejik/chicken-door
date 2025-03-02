/***********************************************************************
 * Filename: servo.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares classes for managing servo mechanisms including Motor,
 *     Encoder, and Servo with advanced control features. It provides
 *     interfaces for motor speed and direction control, position
 *     encoding, and safety mechanisms against overcurrent conditions.
 *     Utilizes PID control for precise position and speed management.
 *
 ***********************************************************************/


#pragma once
#include "stdint.h"
#include "debounce.h"
#include "parameters.h"
#include "exp_filter.h"
#include "pid.h"
#include "error.h"
#include "log.h"

// #define DEBUG_SERVO_INFO

#define MAX_SPEED_CONST 100 /*%**/
#define MIN_SPEED_CONST 50	/*%*/
#define PWM_FREQ_HZ 20000

#define OVERCURRENT_FAST(_limit) (((_limit) * 170) / 100)

#define ENCODER_SAMPLE_PERIOD_MS 2
#define ENCODER_SAMPLE_DEBOUNCE_CNT 5

#define VELOCITY_CONF_MM_S (8 * 1000)

#define MAX_STEADY_TIME 2000

typedef union
{
	uint8_t inputs;
	struct
	{
		uint8_t chA : 1;
		uint8_t chB : 1;
		uint8_t lastchA : 1;
		uint8_t lastchB : 1;
	};

} EncoderInput_t;

class Encoder
{
private:
	EncoderInput_t encInp;
	const int pinA;
	const int pinB;
	const int pinE;
	std::atomic<int16_t> position;
	uint32_t startA;
	uint32_t startB;
	std::atomic<int32_t> steadyTime;
	std::atomic<int16_t> lastPos;
	std::atomic<int16_t> actSpeed;

public:
	Debounce debLimMax;
	Debounce debLimMin;

	Encoder(int pA, int pB, int pE) : pinA(pA), pinB(pB), pinE(pE), steadyTime(0), actSpeed(0), debLimMax(30, 50), debLimMin(30, 50){};

	void Init(void);
	void SensorPwr(bool pwr);
	int16_t Read();
	bool IsSteady(void);
	void SetPosition(int16_t pos);
	int16_t GetPosition(void);
	int16_t GetActSpeed();
};

class Motor
{
private:
	const int pinUp;
	const int pinDown;
	const int pinEn;
	const int pwmCH;

public:
	Motor(int pUp, int pDown, int pEn, int pCH) : pinUp(pUp), pinDown(pDown), pinEn(pEn), pwmCH(pCH){};

	void Init(void);

	void Run(int16_t rate);
};

class Servo
{
private:
	PID PidPosition;
	PID PidSpeed;

	ExpFilter current;
	ExpFilter temp_mv;

	Motor &motor;
	Encoder &sensor;

	bool isStopped;

	void InitMotorCtrl(void);

public:
	std::atomic<int16_t> speedCtrlPWM;

	Servo(Motor &mot, Encoder &enc) : motor(mot), sensor(enc), current(16), temp_mv(8){};

	void IsOverCurrentUp(int16_t current_ma);
	void IsOverCurrentDown(int16_t current_ma);
	void SpeedCtrl(void);
	void Ctrl(void);
	void PositionCtrl(void);
	void Init(void);
	void MotorPWR(bool enable);
	void ReadCurrent(void);
	void ReadTemperature(void);
	int16_t GetTemperature(void);

	void Run(void);

	void Stop(void);
	void ChangePosition(int16_t delta_pos);
	void SetPosition(int16_t pos);

	bool IsPWMActive(void)
	{
		return speedCtrlPWM > MIN_SPEED_CONST;
	}
};