/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TANS_RECEIEVE_BUFF_CONTROL_H
#define __TANS_RECEIEVE_BUFF_CONTROL_H
  /* Includes ------------------------------------------------------------------*/
//处理器有关的头文件
#include "bsp_board.h"
/* USER CODE BEGIN Includes */
//modbus处理过程中使用到的头文件

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

uint8_t Modbus_Master_RB_Initialize(void);
uint8_t Modbus_Master_Rece_Flush(void);
uint8_t Modbus_Master_Rece_Available(void);
uint8_t Modbus_Master_GetByte(uint8_t  *getbyte);
uint8_t Modbus_Master_Rece_Handler(void);
uint8_t Modbus_Master_Read(void);
uint8_t Modbus_Master_Write(uint8_t *buf,uint8_t length);
uint32_t Modbus_Master_Millis(void);
#endif 
/********END OF FILE****/
