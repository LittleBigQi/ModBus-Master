
# MODBUS MASTER RTU在STM32上的实现 #
----------

## 1.概述 ##


- 最近需要将几个信号采集模块通过总线串联起来，这样便于系统模块化。故将目光关注到了工业上经常使用的modbus协议。

- modbus协议是一种一主多从的拓扑结构，主要是应用层软件协议，有关modbus的相关信息，可以自行google、百度。

- STM32实现的Master工程代码在github上，[点击获取](https://github.com/LittleBigQi/ModBus-Master)。

## 2.开发环境 ##

- STM32F042单片机
- MDK-KEIL5
- STM32-CUBE库
- Modbus slaver测试软件


## 3.移植来源 ##

- 信号采集模块作为slaver，采用的是开源的freemodbus协议。关于其的实现大家百度一下都能发现，相关的移植过程介绍也很多，不再一一赘述。值得注意的是：这个freemodbus的源码值得一看，其判断对一帧数据包的接受采用的是定时器判断超时。大体思路是中断接收函数在接收每一个字节数据时会重置定时器，如果定时器发生定时溢出中断，则说明没有新数据到来，代表一个数据包接收完整。

- 然后就是master的设计实现。freemodbus并没有开源的master实现代码，故这部分需要我们自己开发完成。在github上发现有人发布了ardunio版本的master，但是ardunio的代码采C++语言编写，需要我们做一些C语言的移植和一些硬件底层接口的移植。

    > ardunio master的github源工程链接[点击](https://github.com/4-20ma/ModbusMaster)，感谢他的分享。

## 4.移植过程 ##

- 了解ardunio modbus库的实现思路——很简单明了。打开源工程,里面有源代码和例程代码，不过例程代码需要用ardunio的IDE打开。其大体思路就是每个Modbus Function都用一个函数实现，如

		uint8_t ModbusMaster::readDiscreteInputs(uint16_t u16ReadAddress,uint16_t u16BitQty)
		{
		  _u16ReadAddress = u16ReadAddress;
		  _u16ReadQty = u16BitQty;
		  return ModbusMasterTransaction(ku8MBReadDiscreteInputs);
		 }
这个ModbusMasterTransaction函数就是根据用户选择的功能模块填充数组并且发送，然后等待从机回应的数据（带超时检测），接着解析接收到的数据包，如果成功则将数据放在_u16ResponseBuffer数组中
- 将ardunio的C++代码移植为C语言；

- 将ardunio相关的serial等函数使用自己的代码实现，serial函数其实就是硬件层接口的函数封装，这也是移植到其他平台必须要根据自身平台做相应的改变。   
**(1).** 在ModbusMasterTransaction函数中涉及到_serial->read()、_serial->write()、_serial->flush()、_serial->available()、millis()、bitWrite()、bitRead()、word()几种函数，从名字中我们就可以知道什么意思，故我们需要在我们的系统中重新实现这几个函数。  
**(2).** 我们底层串口的设计思路如下，数据发送采用数据的发送直接采用循环发送，而STM32cube库已经将这个功能封装好API接口，我们再封装一层即可，如下所示

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
这句函数就相当于_serial->write()的实现。  
**(3).** 串口数据的接收：我们采用中断接收的方式，并且在中断处理函数中将接收到的字节采用循环队压人缓冲区，这样子就能实现ardunio的功能代码。

    >  跟底层相关的移植代码可到我的工程 Modbus_Master--trans_recieve_buff_control.c .h文件查看。  
    
	_serial->read()的变体为：  

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
_serial->flush()的变体为：

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
_serial->available()的变体为：

		/**
		  * @brief  判断ringbuffer里面是否有尚未处理的字节
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
millis()的变体为：

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
其余几个函数的变体如下：

		/*模拟ardunio函数*************************************************/
		static inline uint8_t lowByte(uint16_t ww)
		{
		  return (uint8_t) ((ww) & 0x00FF);
		}
		
		static inline uint8_t highByte(uint16_t ww)
		{
		  return (uint8_t) ((ww) >> 8);
		}
		
		static inline uint16_t word(uint8_t H_Byte,uint8_t L_Byte)
		{
			uint16_t word;
			word = (uint16_t)(H_Byte<<8);
			word = word + L_Byte;
		  return word;
		}
		
		#define bitSet(value, bit)  ((value) |= (1UL << (bit))) 
		#define bitClear(value, bit)  ((value) &= ~(1UL << (bit)))
		
		#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
		#define bitRead(value, bit)  (((value) >> (bit)) & 0x01) 
**(4).**剩下的就是串口的一些硬件初始化，如波特率等，这些配置由于我是用CUBEMX配置直接生成的代码，这里不做过多阐述。

## 5.代码调试 ##

完成上述关键代码的移植后就可以进行调试了。
 
- 在main函数中增加如下的测试代码

		  while (1)
		  {
				uint8_t result;
				//测试read input registers功能
		        //从机地址0x01 ,连续都2个地址为0x2的寄存器
				result = ModbusMaster_readInputRegisters(0x01,0x2, 2);
				if (result == 0x00)
				{
					Input_Result[0] = ModbusMaster_getResponseBuffer(0x00);
					Input_Result[1] = ModbusMaster_getResponseBuffer(0x01);
				}
					HAL_Delay(1000);
		  }

- 我们使用Modbus Slave软件来模拟从机。并且设置相应的地址里面的数值为1和2。软件使用方法可自行百度、google。下图为软件设置及运行结果。  
![](https://i.imgur.com/FHz8VgM.png)

- 然后我们用keil仿真查看结果Input_Result数组的结果。如下图所示，结果显示modbus通信正确。  
![](https://i.imgur.com/S9kJUOc.png)

## 6.总结 ##

以上即为本次移植的过程，有需要的朋友可以直接使用我的代码，亦可使用ardunio代码自行移植。