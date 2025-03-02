/***********************************************************************
 * Filename: modbus_serial.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Declares the ModbusSerial class, which inherits from ModbusDll
 *     and implements functions for handling Modbus communication
 *     over a serial interface. The file defines constants for buffer
 *     sizes and timing, and includes methods for managing PDU data,
 *     reading and writing Modbus registers, and calculating CRC.
 *
 ***********************************************************************/


#pragma once
#include "common.h"
#include "modbus_slave.h"

#define MODBUS_BUFFERSIZE (256)

#define DEF_ADDR 1
#define DEF_RATE 38400


#define MODBUS_NMR_BITS (1 + 8 + 1) /*start, 8 databits, no parity,1 stop bits*/
/*Min times per Modbus specification*/
#define MIN_INTERBYTE_TIME_US 750
#define MIN_INTERFRAME_TIME_US 1750
#define BYTE_TIME_US(bd) ((uint32_t)(MODBUS_NMR_BITS * 1000000UL) / (bd))

class ModbusSerial : public ModbusDll
{

protected:
	uint8_t Address;
	uint8_t BufferOut[MODBUS_BUFFERSIZE];
	uint8_t BufferIn[MODBUS_BUFFERSIZE];
	size_t inIdx;
	size_t outIdx;
	size_t outLen;
	size_t inpLen;

	void RxEnd(bool bytime);
	void TxEnd(void);
	void RxErr(void);

public:
	ModbusSerial() {}
	void initdll(void);
	void Run(void);
	void sendPDU(void);
	void receivePDU(void);
	PduStatus_t getstatus(void);
	void setdeviceid(uint8_t adr) { Address = adr; }
	// Buffer interface
	void settxlenPDU(size_t len) { outLen = len + 3; outIdx = 1; }
	size_t getrxlenPDU(void) { return inpLen - 3; } /*delka bez adresy a crc*/
	void writebPDU(size_t id, uint8_t val)
	{
		BufferOut[id + 1] = val;
		outIdx = id + 1;
		if (outIdx >= outLen)
		{
			outLen = outIdx + 1;
		}
	}
	void writewPDU(size_t id, uint16_t val)
	{
		BufferOut[id + 1] = (uint8_t)(val >> 8);
		BufferOut[id + 2] = (uint8_t)(val & 0xFF);
		outIdx = id + 2;
		if (outIdx >= outLen)
		{
			outLen = outIdx + 1;
		}
	}
	uint8_t readbPDU(size_t id)
	{
		return BufferIn[id + 1];
	}
	uint16_t readwPDU(size_t id)
	{
		return (uint16_t)BufferIn[id + 1] << 8 | (uint16_t)BufferIn[id + 2];
	}
	size_t maxsizerxPDU(void) { return MODBUS_BUFFERSIZE; }
	size_t maxsizetxPDU(void) { return MODBUS_BUFFERSIZE; }
	uint16_t calcCRC(const uint8_t *pData, size_t len) const;

	void writewPDU(uint16_t val)
	{
		writewPDU(outIdx, val);
	}

	void writebPDU(uint16_t val)
	{
		writebPDU(outIdx, val);
	}

	uint8_t readbPDU(void)
	{
		if (inIdx < inpLen)
		{
			return BufferIn[inIdx++];
		}
		return 0;
	}
	uint16_t readwPDU(void)
	{
		if (inIdx < (inpLen - 1))
		{
			inIdx += 2;
			return readwPDU(inIdx - 2);
		}
		return 0;
	}
};
