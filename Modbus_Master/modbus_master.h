/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_MASTER_H
#define __MODBUS_MASTER_H
  /* Includes ------------------------------------------------------------------*/
//处理器有关的头文件
#include "bsp_board.h"
/* USER CODE BEGIN Includes */
//modbus处理过程中使用到的头文件
#include "crc16.h"
#include "word.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void ModbusMaster_begin(void);
 void ModbusMaster_beginTransmission(uint16_t u16Address);
 uint8_t ModbusMaster_requestFrom(uint16_t address, uint16_t quantity);
 void  ModbusMaster_sendBit(uint8_t data);
 void ModbusMaster_send16(uint16_t data);
 void ModbusMaster_send32(uint32_t data);
 void ModbusMaster_send8(uint8_t data);
 uint8_t ModbusMaster_available(void);
 uint16_t ModbusMaster_receive(void);
 uint16_t ModbusMaster_getResponseBuffer(uint8_t u8Index);
 void ModbusMaster_clearResponseBuffer();
 uint8_t ModbusMaster_setTransmitBuffer(uint8_t u8Index, uint16_t u16Value);
 void ModbusMaster_clearTransmitBuffer();
uint8_t ModbusMaster_readCoils(uint8_t SlaveID,uint16_t u16ReadAddress, uint16_t u16BitQty);
uint8_t ModbusMaster_readDiscreteInputs(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16BitQty);
uint8_t ModbusMaster_readHoldingRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16ReadQty);
uint8_t ModbusMaster_readInputRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint8_t u16ReadQty);	
uint8_t ModbusMaster_writeSingleCoil(uint8_t SlaveID,uint16_t u16WriteAddress, uint8_t u8State);
uint8_t ModbusMaster_writeSingleRegister(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16WriteValue);
uint8_t ModbusMaster_writeMultipleCoils(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16BitQty);
uint8_t ModbusMaster_writeMultipleRegisters(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16WriteQty);
uint8_t ModbusMaster_maskWriteRegister(uint8_t SlaveID,uint16_t u16WriteAddress,uint16_t u16AndMask, uint16_t u16OrMask);
uint8_t ModbusMaster_readWriteMultipleRegisters(uint8_t SlaveID,uint16_t u16ReadAddress,uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty);

#endif 
/********END OF FILE****/
