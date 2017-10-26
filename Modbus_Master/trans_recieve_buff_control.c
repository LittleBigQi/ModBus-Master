#include "trans_recieve_buff_control.h"
#include "ringbuffer.h"

RingBuffer  m_Modbus_Master_RX_RingBuff;
uint8_t     m_Modbus_Master_RX_Buff[200];

extern UART_HandleTypeDef huart2;
/**
  * @brief  初始化中断接收的的ringbuffer环形队列配置,中断接收的字节都用m_Modbus_Master_RX_RingBuff该结构体指针进行管理
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_RB_Initialize(void)
{
	/*初始化ringbuffer相关的配置*/
	rbInitialize(&m_Modbus_Master_RX_RingBuff, m_Modbus_Master_RX_Buff, sizeof(m_Modbus_Master_RX_Buff));
	return 1 ;
}


/**
  * @brief  清除环形队列
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Flush(void)
{
  rbClear(&m_Modbus_Master_RX_RingBuff);
}
/**
  * @brief  判断GPS的ringbuffer里面是否有尚未处理的字节
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Available(void)
{
	/*如果数据包buffer里面溢出了，则清零，重新计数*/
		if(m_Modbus_Master_RX_RingBuff.flagOverflow==1)
		{
			rbClear(&m_Modbus_Master_RX_RingBuff);
		}
	return !rbIsEmpty(&m_Modbus_Master_RX_RingBuff);
}

/****************************************************************************************************/
/*下面是与硬件接口层相关联的部分，根据不同处理器的处理方式进行移植*/
/**
  * @brief  获取接收寄存器里面的数值
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_GetByte(uint8_t  *getbyte)
{
  if(HAL_UART_Receive (&huart2 ,(uint8_t *)getbyte,1,0x01) != HAL_OK )
  {
		return HAL_ERROR;
	}
	else
	{
	  return HAL_OK;
	}
}

/**
  * @brief  中断处理函数，在串口接收中断中调用：将寄存器的数值压人缓冲区
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Handler(void)
{
	 uint8_t byte;
	//读取寄存器里面的数据，并且将数据压入环形队列
   if(Modbus_Master_GetByte(&byte)==HAL_OK)
	 {
	   rbPush(&m_Modbus_Master_RX_RingBuff, (uint8_t)(byte & (uint8_t)0xFFU));
	 }
}

/**
  * @brief  读出缓冲区的数据
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Read(void)
{
	uint8_t cur =0xff;
	if( !rbIsEmpty(&m_Modbus_Master_RX_RingBuff))
	{
		  cur = rbPop(&m_Modbus_Master_RX_RingBuff);
	}
	return cur;
}

/**
  * @brief  将数据包发送出去
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Write(uint8_t *buf,uint8_t length)
{
 if(HAL_UART_Transmit(&huart2 ,(uint8_t *)buf,length,0xff))
 {
   return HAL_ERROR;
 }
 	else
	{
	  return HAL_OK;
	}
}

/**
  * @brief  1ms周期的定时器
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint32_t Modbus_Master_Millis(void)
{
  return HAL_GetTick();
}
