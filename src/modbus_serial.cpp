/***********************************************************************
 * Filename: modbus_serial.cpp
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Implements the ModbusSerial class, which provides functions for 
 *     Modbus communication over a serial interface. 
 *     The class utilizes the Arduino HardwareSerial library and manages 
 *     RS485-specific pin controls and timing.
 *
 ***********************************************************************/


#include "Arduino.h"
#include "modbus_serial.h"
#include "modbus_slave.h"
#include "string.h"
#include <HardwareSerial.h>
#include "pin_map.h"
#include "driver/uart.h"
#include "semphr.h"
#include "parameters.h"

HardwareSerial Rs485(RS485_SERIAL_PORT);
SemaphoreHandle_t  MdbSem = xSemaphoreCreateBinary();
static bool RxEvent;
static bool TxEvent;

#define CRC_INIT_VAL 0xFFFF
#define RS485_BAUDRATE 38400

static const uint16_t CRC16Table[] =
	{0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601,
	 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0,
	 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81,
	 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941,
	 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01,
	 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0,
	 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081,
	 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 0x3C00,
	 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
	 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981,
	 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
	 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700,
	 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0,
	 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281,
	 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01,
	 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1,
	 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80,
	 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
	 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101,
	 0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0,
	 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481,
	 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 0x8801,
	 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1,
	 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581,
	 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341,
	 0x4100, 0x81C1, 0x8081, 0x4040};

uint16_t ModbusSerial::calcCRC(const uint8_t *pData, size_t len) const
{
	uint8_t temp;
	uint16_t CRCWord = CRC_INIT_VAL;
	while (len--)
	{
		temp = *pData++ ^ CRCWord;
		CRCWord >>= 8;
		CRCWord ^= CRC16Table[temp];
	}
	return (uint16_t)CRCWord;
}


void frameReceive(void)
{
	if (digitalRead(DE485))
	{
		digitalWrite(DE485, 0);
		Rs485.flush(false);
		TxEvent = true;
	}
	else
	{
		RxEvent = true;
	}
	xSemaphoreGive(MdbSem);
}


void ModbusSerial::initdll(void)
{
	Rs485.setRxBufferSize(MODBUS_BUFFERSIZE);
	Rs485.begin(RS485_BAUDRATE, SERIAL_8N1, RXD485, TXD485);
	Rs485.setRxTimeout(2);
	Rs485.onReceive(frameReceive, true);

	pinMode(DE485, OUTPUT);
	digitalWrite(DE485, 0);

	Address = DEF_ADDR;

	State = PDUIdle;
}


void ModbusSerial::Run(void)
{

}

void ModbusSerial::sendPDU(void)
{
	BufferOut[0] = Address; /*put slave address before pdu data*/
	uint16_t len = outLen;

	uint16_t crc = calcCRC(BufferOut, len);
	BufferOut[len] = crc & 0xFF;
	BufferOut[len + 1] = crc >> 8;
	TxEvent = false;
	digitalWrite(DE485, 1);
	delayMicroseconds(100); /*wait for DE pin to be stable*/
	Rs485.write(BufferOut, len + 2);

	State = PDUTransmit;
}

void ModbusSerial::TxEnd(void)
{
	digitalWrite(DE485, 0);
}

void ModbusSerial::receivePDU(void)
{
	State = PDUReceive;
}

void ModbusSerial::RxEnd(bool bytime)
{
	if (State == PDUReceive)
	{
		if (BufferIn[0] == Address)
		{
			State = PDUUnicast;
		}
		else
		{

		}
	}
	else
	{

	}
}

void ModbusSerial::RxErr(void)
{
	State = PDUInvalid;
}

PduStatus_t ModbusSerial::getstatus(void)
{
	xSemaphoreTake(MdbSem,pdMS_TO_TICKS(2000));
	PduStatus_t retstate = State;

	if (RxEvent)
	{
		size_t i = Rs485.available();
		if (i > 3)
		{
			if (i < MODBUS_BUFFERSIZE)
			{
				Rs485.readBytes(BufferIn, i);
				inpLen = i;
				if (BufferIn[0] == Address)
				{
					retstate = PDUUnicast;
				}
				Rs485.flush(false);
			}
			else
			{
				Rs485.flush(false);
			}
		}
		else
		{
			TimeoutError.Set(TimeoutError.Get() + 1);
		}
		RxEvent = false;
	}

	switch (retstate)
	{
	case PDUUnicast:
		/*Modbus frame is available in buffer*/
		/* Process received frame */
		{
			uint16_t len = inpLen - 2;
			if (len > 0)
			{
				/*crc je v ramci little endian*/
				uint16_t crc = (uint16_t)BufferIn[len] | (uint16_t)BufferIn[len + 1] << 8;
				if (crc != calcCRC(BufferIn, len))
				{
					retstate = PDUInvalid;
					TxEvent = false;
					RxEvent = false;
					CRCError.Set(CRCError.Get() + 1);
				}
			}
		}

		break;
	default:
		/*there is no operation in progress- idle state*/
		break;
	};

	return retstate;
}
