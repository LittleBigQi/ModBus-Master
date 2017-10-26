#include "ringbuffer.h"
/**
  * @brief  rbInitialize初始化配置，将缓冲区信息填入结构体
  * @param  pRingBuff:ringbuffer结构体
	*         buff：数据缓冲区
	*         length：缓冲区大小
  * @note   
  * @retval void
  * @author xiaodaqi
  */	
void rbInitialize(RingBuffer* pRingBuff, uint8_t* buff, uint16_t length)
{
	pRingBuff->pBuff = buff;
	pRingBuff->pEnd  = buff + length;
	pRingBuff->wp = buff;
	pRingBuff->rp = buff;
	pRingBuff->length = length;
	pRingBuff->flagOverflow = 0;
}

/**
  * @brief  清除ringbuffer结构体的相关信息
  * @param  pRingBuff：待处理的ringbuffer
  * @note   
  * @retval void
  * @author xiaodaqi
  */	

 void rbClear(RingBuffer* pRingBuff)
{
 	pRingBuff->wp = pRingBuff->pBuff;
	pRingBuff->rp = pRingBuff->pBuff;
	pRingBuff->flagOverflow = 0;
}

/**
  * @brief  压入单字节到缓冲区
  * @param  pRingBuff：待处理的ringbuffer
  *         value：压入的数据
  * @note   
  * @retval void
  * @author xiaodaqi
  */	
 void rbPush(RingBuffer* pRingBuff, uint8_t value)
{
	uint8_t* wp_next = pRingBuff->wp + 1;
	if( wp_next == pRingBuff->pEnd ) {
		wp_next -= pRingBuff->length; // Rewind pointer when exceeds bound
	}
	if( wp_next != pRingBuff->rp ) {
		*pRingBuff->wp = value;
		pRingBuff->wp = wp_next;
	} else {
		pRingBuff->flagOverflow = 1;
	}
}

/**
  * @brief  压出单字节到缓冲区
  * @param  pRingBuff：待处理的ringbuffer   
  * @note   
  * @retval 压出的数据
  * @author xiaodaqi
  */	
 uint8_t rbPop(RingBuffer* pRingBuff)
{
	if( pRingBuff->rp == pRingBuff->wp ) return 0; // empty
  
	uint8_t ret = *(pRingBuff->rp++);
	if( pRingBuff->rp == pRingBuff->pEnd ) {
		pRingBuff->rp -= pRingBuff->length; // Rewind pointer when exceeds bound
	}
	return ret;
}

/**
  * @brief  获取缓冲区尚未处理的字节数
  * @param  pRingBuff：待处理的ringbuffer   
  * @note   
  * @retval 待处理的字节数
  * @author xiaodaqi
  */
 uint16_t rbGetCount(const RingBuffer* pRingBuff)
{
	return (pRingBuff->wp - pRingBuff->rp + pRingBuff->length) % pRingBuff->length;
}

/**
  * @brief  判断缓冲区是否为空
  * @param  pRingBuff：待处理的ringbuffer   
  * @note   
  * @retval 空为1；否则为0
  * @author xiaodaqi
  */
 int8_t rbIsEmpty(const RingBuffer* pRingBuff)
{
	return pRingBuff->wp == pRingBuff->rp; 
}

/**
  * @brief  判断缓冲区是否空
  * @param  pRingBuff：待处理的ringbuffer   
  * @note   
  * @retval 满为1；否则为0
  * @author xiaodaqi
  */
 int8_t rbIsFull(const RingBuffer* pRingBuff)
{
 	return (pRingBuff->rp - pRingBuff->wp + pRingBuff->length - 1) % pRingBuff->length == 0;
}
