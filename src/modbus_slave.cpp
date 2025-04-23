/***********************************************************************
 * Filename: modbus_slave.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the ModbusSlave class and its methods.
 *
 ***********************************************************************/

#include "Arduino.h"
#include "modbus_slave.h"
#include "parameters.h"
#include "task.h"

#define ExcFunFlag (0x80)
#define ExcPduLength (2)

#define ReadHRReqNmrMax ((Dll.maxsizerxPDU() - ReadHRReqLen) / 2)					/*Per Modbus specification 0x7D*/
#define WriteMultipleHRNmrMax ((Dll.maxsizetxPDU() - WriteMultipleHRReqLenMin) / 2) /*Per Modbus specification 0x7C*/

ModbusExcCodes_t ModbusSlave::BroadcastProc(void)
{
	return MB_Exc_IllegalFunction;
}

void ModbusSlave::ErrorProc(ModbusExcCodes_t code)
{
}

ModbusExcCodes_t ModbusSlave::UnicastProc(void)
{

	ModbusExcCodes_t exc = MB_Exc_IllegalFunction;
	if (Dll.getrxlenPDU() > 0)
	{
		// vTaskSuspendAll();
		switch (Dll.readbPDU(0))
		{
		case MbFun_ReadHoldingRegs:
			exc = ReadHoldingRegs();
			break;
		case MbFun_WriteSingleReg:
			exc = WriteSingleReg();
			break;
		case MbFun_WriteMultipleRegs:
			exc = WriteMultipleRegs();
			break;
		default:
			/*Unsupported function*/
			break;
		}
		// xTaskResumeAll();
	}

	return exc;
}

ModbusExcCodes_t ModbusSlave::ReadHoldingRegs(void)
{
	ModbusExcCodes_t retexc = MB_Exc_IllegalDataValue;
	if (Dll.getrxlenPDU() == ReadHRReqLen)
	{ /*request has appropriated length*/
		uint16_t regadr = Dll.readwPDU(1);
		uint16_t regnmr = Dll.readwPDU(3);
		/*response*/
		Dll.settxlenPDU(0);
		Dll.writebPDU(0, MbFun_ReadHoldingRegs);

		if ((regnmr >= 1) && (regnmr <= ReadHRReqNmrMax))
		{
			uint16_t reglast = regadr + regnmr - 1; /*last register going to be written*/
			Dll.writebPDU(1, 2 * regnmr);			/*prepare byte count into response*/
			for (uint16_t i = regadr; i <= reglast; i++)
			{
				int16_t regval;
				Register::ReadReg(&regval, i);
				Dll.writewPDU(regval);
			}
			retexc = MB_No_Exc;
		}
	}
	return retexc;
}

ModbusExcCodes_t ModbusSlave::WriteSingleReg(void)
{
	ModbusExcCodes_t exc = MB_Exc_IllegalDataValue;
	if (Dll.getrxlenPDU() == WriteSingleHRReqLen)
	{ /*request has appropriated length*/
		uint16_t regadr = Dll.readwPDU(1);
		uint16_t regval = Dll.readwPDU(3);
		/*response*/
		Dll.settxlenPDU(0);
		Dll.writebPDU(0, MbFun_WriteSingleReg);

		if (Register::IsReg(regadr))
		{ /*Register exists*/
			if (Register::IsWritable(regadr))
			{
				if (Register::WriteReg(regval, regadr) != 0)
				{ /*OK*/
					exc = MB_No_Exc;
					Dll.writewPDU(1, regadr);
					Dll.writewPDU(3, regval);
				}
			}
			else
			{
				exc = MB_Exc_AccessLvlFailure;
			}
		}
		else
		{ /*register address is invalid*/
			exc = MB_Exc_IllegalDataAddress;
		}
	}
	return exc;
}

ModbusExcCodes_t ModbusSlave::WriteMultipleRegs()
{
	ModbusExcCodes_t exc = MB_Exc_IllegalDataValue;
	if (Dll.getrxlenPDU() >= WriteMultipleHRReqLenMin)
	{
		uint16_t regadr = Dll.readwPDU(1);
		uint16_t regnmr = Dll.readwPDU(3);

		if ((regnmr >= 1) && (regnmr <= WriteMultipleHRNmrMax))
		{ /*count of registers is within limits per standard*/
			uint16_t bytecnt = Dll.readbPDU(5);
			if ((Dll.getrxlenPDU() == (WriteMultipleHRReqLenMin + bytecnt)) && (bytecnt == (2 * regnmr)))
			{											/*Count of values match to requested number bytes and registers*/
				uint16_t reglast = regadr + regnmr - 1; /*last register going to be written*/
				/*check if all register addresses exist*/
				bool exist = true;
				exc = MB_No_Exc;
				for (uint16_t i = regadr; i <= reglast; i++)
				{
					if (!Register::IsReg(i))
					{
						exist = false;
						exc = MB_Exc_IllegalDataAddress;
						break;
					}
					if (!Register::IsWritable(i))
					{
						exist = false;
						exc = MB_Exc_AccessLvlFailure;
						break;
					}
				}

				if (exist)
				{ /*writing value into registers*/
					uint16_t written = 0;
					int j = 6;
					for (uint16_t i = regadr; i <= reglast; i++)
					{
						written += Register::WriteReg(Dll.readwPDU(j), i);
						j += 2;
					}
					if (written == regnmr)
					{ /*requested number of register has been written*/
						Dll.settxlenPDU(0);
						Dll.writebPDU(0, MbFun_WriteMultipleRegs);
						Dll.writewPDU(regadr);
						Dll.writewPDU(regnmr);
					}
					else
					{ /* all registers hasn't been successfully written*/
						exc = MB_Exc_IllegalDataValue;
					}
				}
			}
		}
	}
	return exc;
}

void ModbusSlave::Init(void)
{
	Dll.initdll();
}

void ModbusSlave::Run(void)
{
	PduStatus_t rcvstatus;

	ModbusExcCodes_t exc = MB_No_Exc;
	rcvstatus = Dll.getstatus();

	switch (rcvstatus)
	{
	case PDUUnicast: /*Correct message for this slave has been received*/
		exc = UnicastProc();
		if (exc != MB_No_Exc)
		{
			ErrorProc(exc);
			/*Exception response preparing*/
			Dll.writebPDU(0, Dll.readbPDU(0) | ExcFunFlag);
			Dll.writebPDU(1, exc);
			Dll.settxlenPDU(2);

			ExceptionError.Set(ExceptionError.Get() + 1);
		}
		else
		{
			CorrectPackets.Set(CorrectPackets.Get() + 1);
		}
		/*start response transmitting*/
		Dll.sendPDU();
		break;

	case PDUBroadcast: /*Correct message for all slaves has been received*/
		exc = BroadcastProc();
		if ((exc != MB_No_Exc))
		{
			ErrorProc(exc);
			ExceptionError.Set(ExceptionError.Get() + 1);
		}
		else
		{
			CorrectPackets.Set(CorrectPackets.Get() + 1);
		}
		Dll.receivePDU();
		break;
	case PDUInvalid: /*Corrupted message received*/
		ErrorProc(MB_No_Exc);
		ExceptionError.Set(ExceptionError.Get() + 1);
		break;
	case PDUIdle: /*idle state*/
		Dll.receivePDU();
		/*start response transmitting*/
		break;
	default:
		/*RX or TX in progress*/
		break;
	}
}
