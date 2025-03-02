/***********************************************************************
 * Filename: modbus_slave.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares constants, enumerations, and classes for implementing 
 *     a Modbus slave. The ModbusDll class provides an interface for 
 *     managing the data link layer (DLL), and the ModbusSlave class 
 *     implements core Modbus slave functionality, including processing 
 *     Modbus requests and handling errors. 
 *
 ***********************************************************************/


#pragma once
#include "common.h"

#define ReadHRReqLen (5)	 /*Function code+Regadr(2)+Regnmr(2)*/
#define ReadHRRspLenMin (2U) /*Function code+byte#+.....*/

#define ReadSlaveIDReqLen (1U)
#define ReadSlaveIDRspLen (0x28U)

#define WriteSingleHRReqLen (5U) /*Function code+Regadr(2)+Regval(2)*/
#define WriteSingleHRRspLen (5U) /*Function code+Regadr(2)+Regval(2)*/

#define WriteMultipleHRReqLenMin (6U) /*Function code+Regadr(2)+Regnmr(2)+ count bytes...*/
#define WriteMultipleHRRspLen (5U)	  /*Function code+Regadr(2)+Regnmr(2)*/

#define ReadWriteMultipleHRReqLenMin (10U)
#define ReadWriteMultipleHRRspLenMin (2U)

typedef enum
{
	MbFun_ReadHoldingRegs = 3,
	MbFun_WriteSingleReg = 6,
	MbFun_WriteMultipleRegs = 16,
	MbFun_ReadSlaveID = 17,
	MbFun_ReadWriteMultipleRegs = 23,
	MbFun_ReadDefaultRegs = 70,
	MbFun_WriteDefaultReg = 71,
	MbFun_ReadDefaultID = 72
} MbFunction_t;

typedef enum
{
	MB_No_Exc = 0,
	MB_Exc_IllegalFunction = 1,
	MB_Exc_IllegalDataAddress = 2,
	MB_Exc_IllegalDataValue = 3,
	MB_Exc_SlaveDeviceFailure = 4,
	// Residoe specific codes
	MB_Exc_AccessLvlFailure = 0x11 /*invalid access level for register*/
} ModbusExcCodes_t;

typedef enum PduStatus_t
{
	PDUIdle,	  /*no PDU data available*/
	PDUStrange,	  /*data with invalid address in buffer*/
	PDUUnicast,	  /*valid PDU from requested slave*/
	PDUBroadcast, /*valid broadcast PDU*/
	PDUInvalid,	  /*valid address but data PDU Corrupted*/
	PDUTransmit,  /*transmit PDU in progress*/
	PDUReceive	  /*waiting for receiving  or receiving in progress*/
} PduStatus_t;

class ModbusDll
{
protected:
	volatile PduStatus_t State;

public:
	virtual PduStatus_t getstatus(void) = 0;
	virtual void setdeviceid(uint8_t adr) = 0;
	virtual void initdll(void) = 0;
	virtual void settxlenPDU(size_t len) = 0;
	virtual size_t getrxlenPDU(void) = 0;
	virtual size_t maxsizerxPDU(void) = 0;
	virtual size_t maxsizetxPDU(void) = 0;
	virtual void sendPDU(void) = 0;
	virtual void receivePDU(void) = 0;
	virtual void writebPDU(size_t id, uint8_t val) = 0;
	virtual void writewPDU(size_t id, uint16_t val) = 0;
	virtual void writewPDU(uint16_t val) = 0;
	virtual uint8_t readbPDU(size_t id) = 0;
	virtual uint16_t readwPDU(size_t id) = 0;
	virtual uint16_t readwPDU(void) = 0;
};

class ModbusSlave
{
private:
protected:
	ModbusExcCodes_t UnicastProc(void);
	ModbusExcCodes_t BroadcastProc(void);
	void ErrorProc(ModbusExcCodes_t code);
	ModbusExcCodes_t WriteMultipleRegs();
	ModbusExcCodes_t WriteSingleReg();
	ModbusExcCodes_t ReadHoldingRegs();

public:
	ModbusDll &Dll;
	ModbusSlave(ModbusDll &Infc) : Dll(Infc)
	{
	}
	void Init(void);
	void Run(void);
};
