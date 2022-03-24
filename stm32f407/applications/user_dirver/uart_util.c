#include "uart_util.h"
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define SAMPLE_UART1_NAME    "uart1"
#define SAMPLE_UART3_NAME    "uart3"
#define SAMPLE_UART4_NAME    "uart4"
#define SAMPLE_UART5_NAME    "uart5"
#define SAMPLE_UART6_NAME    "uart6"

static UART_RX_MSG  rxMsg[5]={0};
static rt_device_t serial[5]={0};
static char readClose[5]={0};
static void initUARTAPP(void);
static char rx_buffer3[RT_SERIAL_RB_BUFSZ3 + 1];
static char rx_buffer1[RT_SERIAL_RB_BUFSZ1 + 1];
static char rx_buffer4[RT_SERIAL_RB_BUFSZ4 + 1];
static char rx_buffer5[RT_SERIAL_RB_BUFSZ5 + 1];
static char rx_buffer6[RT_SERIAL_RB_BUFSZ6 + 1];


static rt_err_t uart_input1(rt_device_t dev, rt_size_t size);
static rt_err_t uart_input3(rt_device_t dev, rt_size_t size);
static rt_err_t uart_input4(rt_device_t dev, rt_size_t size);
static rt_err_t uart_input5(rt_device_t dev, rt_size_t size);
static rt_err_t uart_input6(rt_device_t dev, rt_size_t size);
//------------------------------------------------------------------
//232--->串口5  主板：NONE   							扩展板：打印机
//TTL--->串口1  主板：扫码器+音频模块			扩展板：扫码器
//TTL--->串口4  主板：蓝牙								扩展板：蓝牙
//485--->串口3  IN1+PO4850CTRL01+串口3 (PB2+PB1)   ---》A
//							IN2+PO4850CTRL02+串口3 (PE7+PE8)   ---》B
//485--->串口6  IN3+PO4850CTRL03+串口6 (PE9+PE10)  ---》C
//							IN4+PO4850CTRL04+串口6 (PE11+PE12) ---》D

//#define RS485_1_EN GET_PIN(B, 1)
//#define RS485_2_EN GET_PIN(E, 8)
//#define RS485_3_EN GET_PIN(E, 10)
//#define RS485_4_EN GET_PIN(E, 12)

//#define RS485_1_CHOOSE  GET_PIN(B, 2)
//#define RS485_2_CHOOSE  GET_PIN(E, 7)
//#define RS485_3_CHOOSE  GET_PIN(E, 9)
//#define RS485_4_CHOOSE  GET_PIN(E, 11)
//------------------------------------------------------------------
static void initUARTAPP(void)
{
	 rxMsg[UART1_INDEX].rx_buffer=rx_buffer1;
	 rxMsg[UART3_INDEX].rx_buffer=rx_buffer3;
	 rxMsg[UART4_INDEX].rx_buffer=rx_buffer4;
	 rxMsg[UART5_INDEX].rx_buffer=rx_buffer5;
	 rxMsg[UART6_INDEX].rx_buffer=rx_buffer6;
}

int Uart_Wirte(int index,unsigned char* data,unsigned int len)
{
    if(serial[index] == NULL)
        return RT_ERROR;
    return rt_device_write(serial[index] , 0, data,len);
}
UART_RX_MSG * getUartStatus(int index)
{
	return &rxMsg[index];
}

void setReadClose(int index,unsigned char closeSwitch)
{
	readClose[index]=closeSwitch;
}


void clearRXMsg(int index)
{
	rxMsg[index].rx_len=0;
	rxMsg[index].rxTime=0;
}
rt_device_t getUartDevice(int index)
{
	return serial[index];
}
//串口6-------------------------------------------------------------
/* 串口6接收数据回调函数 */
static rt_err_t uart_input6(rt_device_t dev, rt_size_t size)
{
		unsigned char buff[128]={0};
    rt_err_t result = RT_EOK;
		int  len=0;
		unsigned char i=UART6_INDEX;
	  char * bashBuff=rx_buffer6;
		len=rt_device_read(dev, 0, buff, size);
		if(readClose[UART6_INDEX])
		{
			return result;
		}
		if((rxMsg[i].rx_len+len)<=RT_SERIAL_RB_BUFSZ6)
		{
			rt_memcpy(bashBuff+rxMsg[i].rx_len,buff,len);
			rxMsg[i].rx_len+=len;
		}
		else if(len<=RT_SERIAL_RB_BUFSZ6){
			rt_memcpy(bashBuff,buff,len);
			rxMsg[i].rx_len=len;
		}
		else
		{
			rt_memcpy(bashBuff,buff,RT_SERIAL_RB_BUFSZ6);
			rxMsg[i].rx_len=len;
		}
		rxMsg[i].rxTime=rt_tick_get();
    return result;
}

int Uart6_Wirte(unsigned char* data,unsigned int len)
{
	return Uart_Wirte(UART6_INDEX,data,len);
}

int Uart6_Init(unsigned int rate)
{
	
   return Uart_Init(UART6_INDEX,rate);
}
//串口5-------------------------------------------------------------
/* 串口5接收数据回调函数 */
static rt_err_t uart_input5(rt_device_t dev, rt_size_t size)
{
		unsigned char buff[128]={0};
    rt_err_t result = RT_EOK;
		int  len=0;
		unsigned char i=UART5_INDEX;
	  char * bashBuff=rx_buffer5;
		len=rt_device_read(dev, 0, buff, size);
		if(readClose[UART5_INDEX])
		{
			return result;
		}
		if((rxMsg[i].rx_len+len)<=RT_SERIAL_RB_BUFSZ5)
		{
			rt_memcpy(bashBuff+rxMsg[i].rx_len,buff,len);
			rxMsg[i].rx_len+=len;
		}
		 else if(len<=RT_SERIAL_RB_BUFSZ5){
			rt_memcpy(bashBuff,buff,len);
			rxMsg[i].rx_len=len;
		}
		else
		{
			rt_memcpy(bashBuff,buff,RT_SERIAL_RB_BUFSZ5);
			rxMsg[i].rx_len=len;
		}
		rxMsg[i].rxTime=rt_tick_get();
    return result;
}

int Uart5_Wirte(unsigned char* data,unsigned int len)
{
	return Uart_Wirte(UART5_INDEX,data,len);
}

int Uart5_Init(unsigned int rate)
{
   return Uart_Init(UART5_INDEX,rate);
}
//串口4-------------------------------------------------------------
/* 串口4接收数据回调函数 */
static rt_err_t uart_input4(rt_device_t dev, rt_size_t size)
{
		unsigned char buff[128]={0};
    rt_err_t result = RT_EOK;
		int  len=0;
		unsigned char i=UART4_INDEX;
	  char * bashBuff=rx_buffer4;
		len=rt_device_read(dev, 0, buff, size);
		if(readClose[UART4_INDEX])
		{
			return result;
		}
		if((rxMsg[i].rx_len+len)<=RT_SERIAL_RB_BUFSZ4)
		{
			rt_memcpy(bashBuff+rxMsg[i].rx_len,buff,len);
			rxMsg[i].rx_len+=len;
		}
		else if(len<=RT_SERIAL_RB_BUFSZ4){
			rt_memcpy(bashBuff,buff,len);
			rxMsg[i].rx_len=len;
		}
		else
		{
			rt_memcpy(bashBuff,buff,RT_SERIAL_RB_BUFSZ4);
			rxMsg[i].rx_len=len;
		}
		rxMsg[i].rxTime=rt_tick_get();
    return result;
}

int Uart4_Wirte(unsigned char* data,unsigned int len)
{
	return Uart_Wirte(UART4_INDEX,data,len);
}

int Uart4_Init(unsigned int rate)
{
   return Uart_Init(UART4_INDEX,rate);
}
//串口3-------------------------------------------------------------
/* 串口3接收数据回调函数 */
static rt_err_t uart_input3(rt_device_t dev, rt_size_t size)
{
		unsigned char buff[128]={0};
    rt_err_t result = RT_EOK;
		int  len=0;
		unsigned char i=UART3_INDEX;
	  char * bashBuff=rx_buffer3;
		len=rt_device_read(dev, 0, buff, size);
		if(readClose[UART3_INDEX])
		{
			return result;
		}
		if((rxMsg[i].rx_len+len)<=RT_SERIAL_RB_BUFSZ3)
		{
			rt_memcpy(bashBuff+rxMsg[i].rx_len,buff,len);
			rxMsg[i].rx_len+=len;
		}
		else if(len<=RT_SERIAL_RB_BUFSZ3){
			rt_memcpy(bashBuff,buff,len);
			rxMsg[i].rx_len=len;
		}
		else
		{
			rt_memcpy(bashBuff,buff,RT_SERIAL_RB_BUFSZ3);
			rxMsg[i].rx_len=len;
		}
		rxMsg[i].rxTime=rt_tick_get();
    return result;
}

int Uart3_Wirte(unsigned char* data,unsigned int len)
{
	return Uart_Wirte(UART3_INDEX,data,len);
}

int Uart3_Init(unsigned int rate)
{
   return Uart_Init(UART3_INDEX,rate);
}
//串口1-------------------------------------------------------------
/* 串口1接收数据回调函数 */
static rt_err_t uart_input1(rt_device_t dev, rt_size_t size)
{
		unsigned char buff[128]={0};
    rt_err_t result = RT_EOK;
		int  len=0;
		unsigned char i=UART1_INDEX;
	  char * bashBuff=rx_buffer1;
		len=rt_device_read(dev, 0, buff, size);
		if(readClose[UART1_INDEX])
		{
			return result;
		}
		if((rxMsg[i].rx_len+len)<=RT_SERIAL_RB_BUFSZ1)
		{
			rt_memcpy(bashBuff+rxMsg[i].rx_len,buff,len);
			rxMsg[i].rx_len+=len;
		}
		else if(len<=RT_SERIAL_RB_BUFSZ1){
			rt_memcpy(bashBuff,buff,len);
			rxMsg[i].rx_len=len;
		}
		else
		{
			rt_memcpy(bashBuff,buff,RT_SERIAL_RB_BUFSZ1);
			rxMsg[i].rx_len=len;
		}
		rxMsg[i].rxTime=rt_tick_get();
    return result;
}

int Uart1_Wirte(unsigned char* data,unsigned int len)
{
	return Uart_Wirte(UART1_INDEX,data,len);
}

int Uart1_Init(unsigned int rate)
{
   return Uart_Init(UART1_INDEX,rate);
}
int Uart_Init(int index,unsigned int rate)
{
    int ret = 0;
	  if(rxMsg[UART1_INDEX].rx_buffer==NULL)
		{
			initUARTAPP();
		}
		if(serial[index]!=NULL)
		{
			   return RT_EOK;
		}
    /* 查找串口设备 */
	  switch(index)
		{
			case UART1_INDEX:
				serial[index] = rt_device_find(SAMPLE_UART1_NAME);
				break;
			case UART6_INDEX:
				serial[index] = rt_device_find(SAMPLE_UART6_NAME);
			break;
			case UART3_INDEX:
				serial[index] = rt_device_find(SAMPLE_UART3_NAME);
			break;
			case UART4_INDEX:
				serial[index] = rt_device_find(SAMPLE_UART4_NAME);
			break;
			case UART5_INDEX:
				serial[index] = rt_device_find(SAMPLE_UART5_NAME);
			break;
		} 
    
    if(serial[index]  == RT_NULL)
    {
        return RT_ERROR;
    }
    
    /* step2：修改串口配置参数 */
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.baud_rate = rate;        //修改波特率为 9600
    config.data_bits = DATA_BITS_8;           //数据位 8
    config.stop_bits = STOP_BITS_1;           //停止位 1
    config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config.parity    = PARITY_NONE;           //无奇偶校验位

    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(serial[index], RT_DEVICE_CTRL_CONFIG, &config);
    
    /* 打开设备 */
    ret = rt_device_open(serial[index],RT_DEVICE_FLAG_DMA_RX);
    
    if(ret != RT_EOK)
        return ret;
		 switch(index)
		{
			case UART1_INDEX:
				   rt_device_set_rx_indicate(serial[index] , uart_input1);
				break;
			case UART6_INDEX:
				    rt_device_set_rx_indicate(serial[index] , uart_input6);
			break;
			case UART3_INDEX:
				    rt_device_set_rx_indicate(serial[index] , uart_input3);
			break;
			case UART4_INDEX:
				    rt_device_set_rx_indicate(serial[index] , uart_input4);
			break;
			case UART5_INDEX:
				    rt_device_set_rx_indicate(serial[index] , uart_input5);
			break;
		} 

    return RT_EOK;
}

 unsigned char orCheck(unsigned char *buff,unsigned char len)
{
	unsigned char i=0;
	unsigned check=0;
	for(i=0;i<len;i++)
	{
		check+=buff[i];
	}
	return check;
}
