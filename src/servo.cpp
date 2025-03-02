/***********************************************************************
 * Filename: servo.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the classes declared in servo.h.
 *
 ***********************************************************************/

#include "Arduino.h"
#include "common.h"
#include "servo.h"
#include "pin_map.h"
#include "parameters.h"

#define R_UP 10.0f
#define VCC 3280.0f
#define SHA_R25 10.0f
#define SHA_B 4150.0f
#define SHA_a0 (1.0f / (25 + 273.15))
#define SHA_a1 (1.0f / SHA_B)

void Encoder::Init(void)
{
	pinMode(pinE, OUTPUT);
	pinMode(pinA, INPUT);
	pinMode(pinB, INPUT);
}

void Encoder::SensorPwr(bool pwr)
{
	digitalWrite(pinE, pwr ? LOW : HIGH);
}

int16_t Encoder::Read()
{
	static bool lastchA = false;
	static bool lastchB = false;
	static uint16_t debStateCnt = 0;
	static int8_t lastDir;
	int8_t actDir;
	bool chA = digitalRead(pinA);
	bool chB = digitalRead(pinB);

	if (chA == lastchA && chB == lastchB)
	{
		if (debStateCnt < ENCODER_SAMPLE_DEBOUNCE_CNT)
		{
			debStateCnt++;
			if (debStateCnt == ENCODER_SAMPLE_DEBOUNCE_CNT)
			{
				encInp.chA = lastchA;
				encInp.chB = lastchB;
			}
		}
	}
	else
	{
		debStateCnt = 0;
		lastchA = chA;
		lastchB = chB;
	}

	// Sequence 2->0->1->3 when moving upwards
	// Sequence 3->1->0->2 when moving downwards
	// enc A is the lower bit, enc B is the upper bit
	if (encInp.chA != encInp.lastchA)
	{
		if (encInp.chA == encInp.chB)
		{
			position--;
		}
		else
		{
			position++;
		}
		debLimMax.Inp(!encInp.chB);
		encInp.lastchA = encInp.chA;
	}

	if (encInp.chB != encInp.lastchB)
	{
		if (encInp.chA == encInp.chB)
		{
			position++;
		}
		else
		{
			position--;
		}
		debLimMin.Inp(!encInp.chA);
		encInp.lastchB = encInp.chB;
	}

	if (abs(lastPos - position) > 2)
	{
		int32_t tmp = (position - lastPos) * VELOCITY_CONF_MM_S;
		tmp = (actSpeed + tmp) / 2;
		actSpeed = tmp / steadyTime;
		lastPos.store(position.load());
		steadyTime = 0;
	}
	else if (IsSteady())
	{
		actSpeed = 0;
	}
	else
	{
		steadyTime += ENCODER_SAMPLE_PERIOD_MS;
	}

	return position;
}

bool Encoder::IsSteady(void)
{
	return steadyTime > MAX_STEADY_TIME;
}

void Encoder::SetPosition(int16_t pos)
{
	lastPos = pos;
	position = pos;
}

int16_t Encoder::GetPosition(void)
{
	return position;
}

int16_t Encoder::GetActSpeed()
{
	return actSpeed;
}

void Motor::Init(void)
{
	pinMode(pinDown, OUTPUT);
	pinMode(pinUp, OUTPUT);
	pinMode(pinEn, OUTPUT);

	digitalWrite(pinDown, LOW);
	digitalWrite(pinUp, LOW);
	ledcSetup(pwmCH, PWM_FREQ_HZ, 8);
	ledcAttachPin(pinEn, 0);
	ledcWrite(pwmCH, 0);

	Run(0);
}

void Motor::Run(int16_t rate)
{
	int16_t tmp = rate;
	tmp = tmp * 255;
	tmp = tmp / 100;

	if (rate > 0)
	{
		if (tmp > 255)
		{
			tmp = 255;
		}
		digitalWrite(pinUp, HIGH);
		digitalWrite(pinDown, LOW);
		ledcWrite(pwmCH, tmp);
	}
	else if (rate < 0)
	{
		tmp = -tmp;
		if (tmp > 255)
		{
			tmp = 255;
		}
		digitalWrite(pinUp, LOW);
		digitalWrite(pinDown, HIGH);
		ledcWrite(pwmCH, tmp);
	}
	else
	{
		tmp = 0;
		digitalWrite(pinUp, LOW);
		digitalWrite(pinDown, LOW);
		ledcWrite(pwmCH, tmp);
	}
}

void Servo::InitMotorCtrl(void)
{
	pinMode(MOTOR_RELAY, OUTPUT);
	digitalWrite(MOTOR_RELAY, LOW);
}

void Servo::ReadCurrent(void)
{
	int32_t curr = analogReadMilliVolts(MOTOR_ISENSE) * 100 / 165;
	AktualniProud_mA.Set(current.Input(curr));
}

void Servo::ReadTemperature(void)
{
	temp_mv.Input(analogReadMilliVolts(TEMP_R));
}

int16_t Servo::GetTemperature(void)
{
	float resT = VCC / temp_mv.Out() - 1;
	resT = R_UP / resT;
	resT = 1 / (SHA_a0 + SHA_a1 * logf(resT / SHA_R25)) - 273.15;

	return (int16_t)resT;
}

void Servo::IsOverCurrentUp(int16_t current_ma)
{
	int16_t pwm = abs(RizeniMotoru_PWM.Get());
	if (pwm >= 0 && pwm < 40)
	{
		pwm = 40;
	}

	int32_t limit = (int32_t)MotorProudOtMax.Get() * pwm;
	limit /= 100;
	ovrl_up.Check(current_ma > limit);
	fast_ovrl_up.Check(current_ma > (int16_t)(OVERCURRENT_FAST(limit)));
}

void Servo::IsOverCurrentDown(int16_t current_ma)
{
	int16_t pwm = abs(RizeniMotoru_PWM.Get());
	if (pwm >= 0 && pwm < 40)
	{
		pwm = 40;
	}

	int32_t limit = (int32_t)MotorProudOtMin.Get() * pwm;
	limit /= 100;
	ovrl_down.Check(current_ma > limit);
	fast_ovrl_down.Check(current_ma > (int16_t)(OVERCURRENT_FAST(limit)));
}

void Servo::Init(void)
{
	isStopped = true;
	motor.Init();
	sensor.Init();
	InitMotorCtrl();
	sensor.SensorPwr(true);
	PidSpeed.Setparam(PIDRychlostP.Get(), PIDRychlostI.Get());
	PidSpeed.Outrange(-MAX_SPEED_CONST, MAX_SPEED_CONST, -45, 50);
	PidPosition.Setparam(PIDPolohaP.Get(), 0);
	PidPosition.Outrange(-MaxRychlost.Get(), MaxRychlost.Get(), -MinRychlost.Get(), MinRychlost.Get());
	MotorPWR(true);
}

void Servo::MotorPWR(bool enable)
{
	digitalWrite(MOTOR_RELAY, (enable) ? HIGH : LOW);
}

void Servo::Ctrl(void)
{
	PidPosition.Setparam(PIDPolohaP.Get(), 0);
	PidSpeed.Setparam(PIDRychlostP.Get(), PIDRychlostI.Get());
	PidPosition.Outrange(-MaxRychlost.Get(), MaxRychlost.Get(), -MinRychlost.Get(), MinRychlost.Get());

	PositionCtrl();
	SpeedCtrl();
}

void Servo::Run(void)
{

	ReadCurrent();
	ReadTemperature();

	int32_t DmdSpeed = PozadovanaRychlost.Get();

	if (DmdSpeed > 0)
	{
		encoder_lim_max.Check(sensor.debLimMax.Out());
		encoder_lim_min.Check(false);
		sensor.debLimMin.Reset();

		IsOverCurrentUp(AktualniProud_mA.Get());
		ovrl_down.Check(false);
		fast_ovrl_down.Check(false);

		if (Error::IsBlocked(MOVE_UP | MOVE_CTRL_BLOCK))
		{
			speedCtrlPWM = 0;
		}
	}
	else if (DmdSpeed < 0)
	{
		encoder_lim_min.Check(sensor.debLimMin.Out());
		encoder_lim_max.Check(false);
		sensor.debLimMax.Reset();

		IsOverCurrentDown(AktualniProud_mA.Get());
		ovrl_up.Check(false);
		fast_ovrl_up.Check(false);

		if (Error::IsBlocked(MOVE_DOWN | MOVE_CTRL_BLOCK))
		{
			speedCtrlPWM = 0;
		}
	}
	else
	{
		speedCtrlPWM = 0;
		ovrl_down.Check(false);
		fast_ovrl_down.Check(false);
		ovrl_up.Check(false);
		fast_ovrl_up.Check(false);
	}

	RizeniMotoru_PWM.Set(speedCtrlPWM);
	motor.Run(RizeniMotoru_PWM.Get());
}

void Servo::SpeedCtrl(void)
{
	int32_t ActSpeed = sensor.GetActSpeed();
	int32_t DmdSpeed = PozadovanaRychlost.Get();
	AktualniRychlost.Set(ActSpeed);
	int32_t err = (DmdSpeed - ActSpeed);

	if (Error::IsBlocked(MOVE_UP | MOVE_DOWN))
	{
		speedCtrlPWM = PidSpeed.Setout(0, 0);
	}
	else
	{
		speedCtrlPWM = PidSpeed.Evaluate(err);
	}
}

/*Regulace polohy serva*/
void Servo::PositionCtrl(void)
{
	static int16_t lastDemPos = 0;

	int32_t Err = PozadovanaPoloha_puls.Get() - AktualniPoloha_puls.Get();

	target_pos.Check((abs(Err) < 3) && !isStopped);

	if (Error::IsBlocked(MOVE_UP | MOVE_DOWN))
	{
		PozadovanaRychlost.Set(0);
		speedCtrlPWM = PidSpeed.Setout(0, 0);
	}
	else
	{
		PozadovanaRychlost.Set(PidPosition.Evaluate(Err));
	}

	no_move.Check(sensor.IsSteady() && (PozadovanaRychlost.Get() != 0));
}

void Servo::Stop(void)
{
	PozadovanaPoloha_puls.Set(AktualniPoloha_puls.Get());
	PozadovanaRychlost.Set(0);
	speedCtrlPWM = PidSpeed.Setout(0, 0);
	isStopped = true;
	delay(500);
	PozadovanaPoloha_puls.Set(AktualniPoloha_puls.Get());
}

void Servo::ChangePosition(int16_t delta_pos)
{
	SetPosition(AktualniPoloha_puls.Get() + delta_pos);
}

void Servo::SetPosition(int16_t pos)
{
	PozadovanaPoloha_puls.SetLimit(pos);
	if (pos != AktualniPoloha_puls.Get())
	{
		isStopped = false;
		Error::ClearActionAll(MOVE_UP | MOVE_DOWN);
	}
}