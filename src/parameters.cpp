/****************************************************************************
 * File: parameters.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     This source file implements the initialization and management of system
 *     parameters defined in parameters.h and parameters_table.h. It initializes parameters
 *     as defined in parameters_table.h and provides mechanisms for their registration,
 *     update, and synchronization across different modules of the application.
 *
 ****************************************************************************/

#include "common.h"
#include "parameters.h"
#include "Preferences.h"

#define PAR_DEF_INCLUDES
#include "parameters_table.h"
#undef PAR_DEF_INCLUDES

extern Encoder encoder;
extern Servo servo;

Preferences nv_data;

#undef PAR_DEF_INCLUDES
#undef U32_
#undef S32_
#undef S16_
#undef U16_
#undef STRING_
#undef DefPar_Ram
#undef DefPar_Fun
#undef DefPar_Nv
#define DefPar_Ram(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) _name_##id,
#define DefPar_Nv(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) _name_##id,
#define DefPar_Fun(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_, _fun_) _name_##id,
typedef enum
{
#include "parameters_table.h"
	nmr_parameters
} parname_e;

//*****************************************************************************
//!  Initialization of parameters
//*****************************************************************************
#undef PAR_DEF_INCLUDES
#undef U32_
#undef S32_
#undef S16_
#undef U16_
#undef STRING_
#define U32_ Par_U32
#define S32_ Par_S32
#define S16_ Par_S16
#define U16_ Par_U16
#define STRING_ Par_STRING
#undef DefPar_Ram
#undef DefPar_Fun
#undef DefPar_Nv
#define DefPar_Ram(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) \
	{.min = _min_, .max = _max_, .def = _def_, .dsc = (ParDscr_t)(_type_ | _dir_ | _lvl_), .adr = _regadr_, .atr = _atr_, .ptxt = #_name_},
#define DefPar_Nv(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) \
	{.min = _min_, .max = _max_, .def = _def_, .dsc = (ParDscr_t)(_type_ | _dir_ | _lvl_ | ParNv), .adr = _regadr_, .atr = _atr_, .ptxt = #_name_},
#define DefPar_Fun(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_, _fun_) \
	{.min = _min_, .max = _max_, .def = _def_, .dsc = (ParDscr_t)(_type_ | _dir_ | _lvl_ | ParFun), .adr = _regadr_, .atr = _atr_, .ptxt = #_name_},
const pardef_t Register::ParDef[] =
	{
#include "parameters_table.h"
};
#define idx(_name_) _name_##id
#define ref(_name_) (Register::ParDef[idx(_name_)])
#define par(_name_) _name_##ref(_name_)

#undef PAR_DEF_INCLUDES
#undef U32_
#undef S32_
#undef S16_
#undef U16_
#undef STRING_
#define U32_
#define S32_ int32_reg
#define S16_ int16_reg
#define U16_ uint16_reg
#define STRING_ string_reg
#undef DefPar_Ram
#undef DefPar_Fun
#undef DefPar_Nv
#define DefPar_Ram(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) _type_ _name_(Register::ParDef[_name_##id]);
#define DefPar_Nv(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_)
#define DefPar_Fun(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_, _fun_) _fun_ _name_(Register::ParDef[_name_##id]);
#undef PAR_DEF_INCLUDES
#include "parameters_table.h"

#undef U32_
#undef S32_
#undef S16_
#undef U16_
#undef STRING_
#define U32_
#define S32_ int32_reg_nv
#define S16_ int16_reg_nv
#define U16_ uint16_reg_nv
#define STRING_ string_reg_nv
#undef DefPar_Ram
#undef DefPar_Fun
#undef DefPar_Nv
#define DefPar_Ram(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_)
#define DefPar_Nv(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) \
	_type_ _name_(Register::ParDef[_name_##id]);
#define DefPar_Fun(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_, _fun_)
#include "parameters_table.h"

//
//*****************************************************************************
//! Initialize arrays references to parameters
//*****************************************************************************
#undef U32_
#undef S32_
#undef S16_
#undef U16_
#undef STRING_
#define U32_
#define S32_
#define S16_
#define U16_
#define STRING_
#undef DefPar_Ram
#undef DefPar_Fun
#undef DefPar_Nv
#define DefPar_Ram(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) &_name_,
#define DefPar_Nv(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_) &_name_,
#define DefPar_Fun(_name_, _regadr_, _def_, _min_, _max_, _type_, _dir_, _lvl_, _atr_, _fun_) &_name_,
Register *const Register::ParSet[] =
	{
#include "parameters_table.h"
};

size_t Register::ActiveIdx = nmr_parameters;
uint8_t Register::ActiveLevel = Par_Public;
uint16_t Register::ActiveRegAdr = INVALID_REGADR;
String Register::writeString;
const uint16_t Register::NmrParameters = nmr_parameters;

Register *Register::GetPar(uint16_t Radr)
{

	for (uint16_t i = 0; i < nmr_parameters; i++)
	{
		if (ActiveIdx >= nmr_parameters)
		{
			ActiveIdx = 0;
		}
		uint16_t hr = ParDef[ActiveIdx].adr;
		if (hr == Radr)
		{
			ActiveRegAdr = Radr;
			return ParSet[ActiveIdx];
		}

		if ((Radr > hr) && (Radr < (hr + ParSet[ActiveIdx]->GetSize())))
		{
			ActiveRegAdr = Radr;
			return ParSet[ActiveIdx];
		}
		ActiveIdx++;
	}
	ActiveIdx = nmr_parameters;
	ActiveRegAdr = INVALID_REGADR;
	return NULL;
}

Register *Register::GetParByIdx(uint16_t idx)
{
	if (idx < nmr_parameters)
	{
		return ParSet[idx];
	}
	return NULL;
}

uint8_t Register::ReadReg(int16_t *out, size_t adr)
{
	uint8_t nmr = 0;
	Register *pReg = GetPar(adr);
	if (pReg != NULL && pReg->IsReadable())
	{
		ActiveRegAdr = adr;
		nmr = pReg->GetRegVal(out);
		ActiveRegAdr = INVALID_REGADR;
	}
	else
	{
		*out = NONDEF_REG_VAL;
	}
	return nmr;
}

Register *const Register::ParameterSearch(const String &name)
{
	Register *retval = NULL;
	size_t n = sizeof(ParDef) / sizeof(pardef_t);
	for (int i = 0; i < n; i++)
	{
		if (!name.compareTo(ParDef[i].ptxt))
		{
			retval = ParSet[i];
			break;
		}
	}
	return retval;
}

bool Register::JsonRead(const String &name, JsonObject doc)
{
	Register *reg = ParameterSearch(name);
	if (reg != NULL && reg->IsReadable())
	{
		reg->GetJsonVal(doc[name].to<JsonVariant>());
		return true;
	}
	return false;
}

bool Register::JsonWrite(JsonPair pair)
{
	Register *reg = ParameterSearch(pair.key().c_str());
	if (reg != NULL && reg->IsWritable())
	{
		return reg->SetJsonVal(pair.value());
	}
	return false;
}

bool Register::JsonWrite(const String &key, JsonVariant value)
{
	Register *reg = ParameterSearch(key.c_str());
	if (reg != NULL && reg->IsWritable())
	{
		return reg->SetJsonVal(value);
	}
	return false;
}

bool Register::IsReg(size_t adr)
{
	return GetPar(adr) != NULL;
}

bool Register::IsWritable(size_t adr)
{
	bool retval = false;
	Register *pReg = GetPar(adr);

	if (pReg != NULL)
	{
		retval = pReg->IsWritable();
	}
	return retval;
}

bool Register::IsReadable(size_t adr)
{
	bool retval = false;
	Register *pReg = GetPar(adr);

	if (pReg != NULL)
	{
		retval = pReg->IsReadable();
	}
	return retval;
}

uint8_t Register::WriteReg(int16_t out, size_t adr)
{
	uint8_t nmr = 0;
	Register *pReg = GetPar(adr);
	if (pReg->IsWritable())
	{
		nmr = pReg->SetRegVal(out);
	}
	return nmr;
}

void Register::InitAll(void)
{
	nv_data.begin("nv_data", false);
	for (size_t i = 0; i < nmr_parameters; i++)
	{
		ParSet[i]->ResetVal();
	}
}

//*****************************************************************************
//! \odvozena trida parametru typu S16- registru
//*****************************************************************************
int32_t int16_reg::Get(void)
{
	return value;
}
bool int16_reg::Set(int32_t v)
{
	int16_t tmp;
	if (v > INT16_MAX)
	{
		tmp = INT16_MAX;
	}
	else if (v < INT16_MIN)
	{
		tmp = INT16_MIN;
	}
	else
	{
		tmp = (int16_t)v;
	}
	bool retval = false;
	if (value != tmp)
	{
		retval = true;
		value = tmp;
	}
	return retval;
}
uint8_t int16_reg::GetRegVal(int16_t *out)
{
	*out = (int16_t)value;
	return 1;
}
uint8_t int16_reg::SetRegVal(int16_t inp)
{
	if (this->CheckLimits((int32_t)inp))
	{
		this->Set((int32_t)inp);
		return 1;
	}
	return 0;
}

uint8_t int16_reg::GetRegVal(int16_t *out, uint16_t addr)
{
	return this->GetRegVal(out);
}
uint8_t int16_reg::SetRegVal(int16_t inp, uint16_t addr)
{
	return this->SetRegVal(inp);
}

bool int16_reg::SetLimit(int32_t v)
{
	if (v > def.max)
	{
		return this->Set(def.max);
	}
	if (v < def.min)
	{
		return this->Set(def.min);
	}
	return this->Set(v);
}

void int16_reg::ResetVal(void)
{
	value = (int16_t)def.def;
}

bool int16_reg_nv::Set(int32_t v)
{
	bool retval = int16_reg::Set(v);
	if (retval)
	{
		nv_data.putShort(String(def.adr).c_str(), (int16_t)value);
	}
	return retval;
}
void int16_reg_nv::ResetVal(void)
{
	value = nv_data.getShort(String(def.adr).c_str(), (int16_t)def.def);
}

bool uint16_reg_nv::Set(int32_t v)
{
	bool retval = uint16_reg::Set(v);
	if (retval)
	{
		nv_data.putUShort(String(def.adr).c_str(), (uint16_t)value);
	}
	return retval;
}
void uint16_reg_nv::ResetVal(void)
{
	value = nv_data.getUShort(String(def.adr).c_str(), (uint16_t)def.def);
}

//*****************************************************************************
//! \odvozena trida parametru typu S32- registru
//*****************************************************************************
int32_t int32_reg::Get(void)
{
	return value;
}
bool int32_reg::Set(int32_t v)
{
	bool retval = value != v;
	if (retval)
	{
		value = v;
	}
	return retval;
}
uint8_t int32_reg::GetRegVal(int16_t *out)
{
	size_t idx = getidx();
	static int32_t tmpVal;
	if (idx == 0)
	{
		tmpVal = value;
		*out = (int16_t)((uint32_t)tmpVal >> 16);
	}
	else if (idx == 1)
	{
		*out = (int16_t)((uint32_t)tmpVal & 0xffff);
	}
	return 1;
}
uint8_t int32_reg::SetRegVal(int16_t inp)
{
	static uint16_t highReg = 0;
	size_t idx = getidx();
	if (idx == 0)
	{
		highReg = (uint16_t)inp;
	}
	else if (idx == 1)
	{
		int32_t tmp = (int32_t)(((uint32_t)highReg << 16) | (uint32_t)((uint16_t)inp));
		if (this->CheckLimits(tmp))
		{
			this->Set(tmp);
		}
	}
	return 1;
}

uint8_t int32_reg::GetRegVal(int16_t *out, uint16_t addr)
{
	size_t idx = getidx(addr);
	int32_t tmpVal = value;
	if (idx == 0)
	{
		*out = (int16_t)((uint32_t)tmpVal >> 16);
	}
	else if (idx == 1)
	{
		*out = (int16_t)((uint32_t)tmpVal & 0xffff);
	}
	return 1;
}
uint8_t int32_reg::SetRegVal(int16_t inp, uint16_t addr)
{
	size_t idx = getidx(addr);
	if (idx == 0)
	{
		tmp_high_reg = (uint16_t)inp;
	}
	else if (idx == 1)
	{
		int32_t tmp = (int32_t)(((uint32_t)tmp_high_reg << 16) | (uint32_t)((uint16_t)inp));
		if (this->CheckLimits(tmp))
		{
			this->Set(tmp);
		}
	}
	return 1;
}

bool int32_reg::SetLimit(int32_t v)
{
	if (v > def.max)
	{
		return this->Set(def.max);
	}
	if (v < def.min)
	{
		return this->Set(def.min);
	}
	return this->Set(v);
}
void int32_reg::ResetVal(void)
{
	value = def.def;
}

//*****************************************************************************
//! \odvozena trida parametru typu S32- registru
//*****************************************************************************

bool int32_reg_nv::Set(int32_t v)
{
	bool retval = int32_reg::Set(v);
	if (retval)
	{
		nv_data.putLong(String(def.adr).c_str(), value);
	}
	return retval;
}

void int32_reg_nv::ResetVal(void)
{
	value = nv_data.getLong(String(def.adr).c_str(), def.def);
}

//*****************************************************************************
//! \odvozena trida parametru pro ukladani logu
//*****************************************************************************

uint8_t log_reg::GetRegVal(int16_t *out)
{
	size_t idx = getidx();
	*out = history[idx];
	return 1;
}
uint8_t log_reg::SetRegVal(int16_t inp)
{
	return 0;
}

void log_reg::ResetVal(void)
{
	if (!nv_data.getBytes(String(def.adr).c_str(), history, sizeof(history)))
	{
		memset(history, 0, sizeof(history));
	}
}

void log_reg::Set(ErrorState_t err)
{
	if (err)
	{
		memmove(&history[1], history, (ERR_HISTORY_CNT - 1) * sizeof(ErrorState_t));
		history[0] = err;
		nv_data.putBytes(String(def.adr).c_str(), history, sizeof(history));
	}
}

//*****************************************************************************
//! \odvozena trida parametru pro ziskani hodnoty enkoderu
//*****************************************************************************

int16_t enc_reg::Get(void)
{
	return encoder.GetPosition();
}
uint8_t enc_reg::GetRegVal(int16_t *out)
{
	*out = Get();
	return 1;
}
uint8_t enc_reg::SetRegVal(int16_t inp)
{
	return 0;
}

void enc_reg::ResetVal(void)
{
	encoder.SetPosition(0);
}

int16_t temp_reg::Get(void)
{
	return servo.GetTemperature() + TeplotaKorekce.Get();
}

//*****************************************************************************
//! \odvozena trida parametru typu STRING
//*****************************************************************************

uint8_t string_reg::GetRegVal(int16_t *out)
{
	size_t idx = getidx() * 2;
	if (idx == 0)
	{
		std::lock_guard<std::mutex> lock(mutex);
		writeString = val;
	}
	*out = (int16_t)((uint16_t)writeString.charAt(idx + 1) << 8 | (uint16_t)writeString.charAt(idx));
	return 1;
}

uint8_t string_reg::SetRegVal(int16_t inp)
{
	size_t idx = getidx();
	if (idx < GetSize())
	{
		if (idx == 0)
		{
			writeString.clear();
		}

		bool end = (idx * 2 > writeString.length());

		if (!end)
		{
			bool save = false;
			char tc = ((uint16_t)inp) & 0xFF;
			save |= tc == '\0';

			if (!save)
				writeString += tc;

			tc = ((uint16_t)inp >> 8) & 0xFF;
			save |= tc == '\0';

			if (!save)
				writeString += tc;

			change = true;
			save |= (idx == (GetSize() - 1));

			if (save)
			{
				this->Set(writeString);
			}
		}

		return 1;
	}
	return 0;
}

uint8_t string_reg::GetRegVal(int16_t *out, uint16_t addr)
{
	size_t idx = getidx(addr) * 2;
	if (idx == 0)
	{
		std::lock_guard<std::mutex> lock(mutex);
		tmpString = val;
	}
	*out = (int16_t)((uint16_t)tmpString.charAt(idx + 1) << 8 | (uint16_t)tmpString.charAt(idx));
	return 1;
}
uint8_t string_reg::SetRegVal(int16_t inp, uint16_t addr)
{
	size_t idx = getidx(addr);
	if (idx < GetSize())
	{
		if (idx == 0)
		{
			tmpString.clear();
		}

		bool end = (idx * 2 > tmpString.length());

		if (!end)
		{
			bool save = false;
			char tc = ((uint16_t)inp) & 0xFF;
			save |= tc == '\0';

			if (!save)
				tmpString += tc;

			tc = ((uint16_t)inp >> 8) & 0xFF;
			save |= tc == '\0';

			if (!save)
				tmpString += tc;

			change = true;
			save |= (idx == (GetSize() - 1));

			if (save)
			{
				this->Set(tmpString);
			}
		}

		return 1;
	}
	return 0;
}

void string_reg::ResetVal(void)
{
	val = "";
}

bool string_reg::Set(String &txt)
{
	std::lock_guard<std::mutex> lock(mutex);

	bool retval = val.compareTo(txt);
	if (retval)
	{
		val = txt;
	}
	return retval;
}

bool string_reg::Set(const char *txt)
{
	String tmp = String(txt);
	return this->Set(tmp);
}

String string_reg::Get(void)
{
	std::lock_guard<std::mutex> lock(mutex);
	return val;
}

//*****************************************************************************
//! \odvozena trida parametru typu STRING NV registru
//*****************************************************************************

void string_reg_nv::ResetVal(void)
{
	string_reg::ResetVal();
	val = nv_data.getString(String(def.adr).c_str(), val);
}

// bool ischange(void);
bool string_reg_nv::Set(String &txt)
{
	bool retval = string_reg::Set(txt);
	if (retval)
	{
		std::lock_guard<std::mutex> lock(mutex);
		nv_data.putString(String(def.adr).c_str(), val);
	}
	return retval;
}

//*****************************************************************************
//! \odvozena trida parametru typu STRING NV - pro ulozeni IP adresy
//*****************************************************************************

void ipv4_reg_nv::ResetVal(void)
{
	IPAddress tmp((uint32_t)def.def);
	val = tmp.toString();

	string_reg_nv::ResetVal();
}

bool ipv4_reg_nv::Set(String &txt)
{
	IPAddress tmp;
	bool retval = tmp.fromString(txt);
	if (retval)
	{
		String temp = tmp.toString();
		retval = string_reg_nv::Set(temp);
	}

	return retval;
}

IPAddress ipv4_reg_nv::GetIP()
{
	IPAddress ip;
	{
		std::lock_guard<std::mutex> lock(mutex);
		ip.fromString(val);
	}
	return ip;
}

//*****************************************************************************
//! \odvozena trida parametru typu U16 pro povely otevreni a zavreni
//*****************************************************************************

uint8_t event_reg::SetRegVal(int16_t inp)
{
	if (this->CheckLimits((int32_t)inp))
	{
		uint16_reg::Set((int32_t)inp);
		DoorCtrl::EventSourced((DoorEvents_t)inp, povel_modbus);
	}
	return 1;
}

bool event_reg::Set(int32_t v)
{
	uint16_reg::Set(v);
	DoorCtrl::EventSourced((DoorEvents_t)v, povel_webem);
	return true;
}

//*****************************************************************************
//! \odvozena trida parametru typu U16 pro
//*****************************************************************************

uint8_t statusbits_reg::GetRegVal(int16_t *out)
{
	uint16_t status = StavDvirka.Get();
	typedef enum
	{
		SOtevirani = 1,
		SZavirani = 2,
		SOteviraniMax = 4,
		SZaviraniMin = 8,
		SChybaDveri = 0x8000
	} DoorStatusBits_t;
	uint16_t res = 0;
	switch (status)
	{
	case NeznaznamaPoloha:
		break;
	case Otevirani:
		res |= SOtevirani;
		break;
	case OteviraniMax:
		res |= SOtevirani;
		res |= SOteviraniMax;
		break;
	case Otevreno:
		res |= SOtevreno;
		break;
	case PreruseniOtevirani:
		break;
	case UvolneniOtevirani:
		break;

	case Zavirani:
		res |= SZavirani;
		break;
	case ZaviraniMin:
		res |= SZavirani;
		res |= SZaviraniMin;
		break;
	case Zavreno:
		res |= SZavreno;
		break;
	case PreruseniZavirani:
		break;
	case UvolneniZavirani:
		break;
	case MeziPoloha:
		break;

	case StopZavirani:
		break;
	case StopOtevirani:
		break;
	case ChybaDveri:
		res |= SChybaDveri;
		break;
	default:
		break;
	}
	*out = res;
	return 1;
}
uint8_t statusbits_reg::SetRegVal(int16_t inp)
{
	return 0;
}

//*****************************************************************************
//! \odvozena trida parametru typu STRING pro ukladani casu
//*****************************************************************************
void time_reg_nv::ResetVal(void)
{
	time_reg::Set(nv_data.getLong(String(def.adr).c_str(), def.def));
}
bool time_reg_nv::Set(String &txt)
{
	bool retval = time_reg::Set(txt);
	if (retval)
	{
		nv_data.putLong(String(def.adr).c_str(), t_val);
	}
	return retval;
}
bool time_reg_nv::Set(time_t v)
{
	bool retval = time_reg::Set(v);
	if (retval)
	{
		nv_data.putLong(String(def.adr).c_str(), t_val);
	}
	return retval;
}

bool pwd_reg::Set(int32_t v)
{
	bool retval = int16_reg::Set(v);

	if (value == PinServis.Get())
	{
		ActiveLevel = Par_Installer;
		AccessLvl.Set(Servis);
	}
	else
	{
		ActiveLevel = Par_Public;
		AccessLvl.Set(Uzivatel);
	}

	return retval;
}
bool pwd_reg::CheckLimits(int32_t vl)
{
	if (ActiveLevel == Par_Installer)
	{
		return vl >= def.min && vl <= def.max;
	}
	else
	{
		return vl >= def.min && vl <= def.max && vl == PinServis.Get();
	}
}