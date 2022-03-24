#include "rs485.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "uart_util.h" 

#define EN_TEST_CODE    0

#define RS485_1_EN GET_PIN(B, 1)
#define RS485_2_EN GET_PIN(E, 8)
#define RS485_3_EN GET_PIN(E, 10)
#define RS485_4_EN GET_PIN(E, 12)
#define RS485_5_EN GET_PIN(A, 11)
#define RS485_6_EN GET_PIN(D, 1)
#define RS485_7_EN GET_PIN(E, 15)
#define RS485_8_EN GET_PIN(E, 15)

#define RS485_1_CHOOSE  GET_PIN(B, 2)
#define RS485_2_CHOOSE  GET_PIN(E, 7)
#define RS485_3_CHOOSE  GET_PIN(E, 9)
#define RS485_4_CHOOSE  GET_PIN(E, 11)
#define RS485_5_CHOOSE  GET_PIN(A, 8)
#define RS485_6_CHOOSE  GET_PIN(D, 0)
#define RS485_7_CHOOSE  GET_PIN(E, 13)
#define RS485_8_CHOOSE  GET_PIN(E, 14)

/*波特率*/
#define RS485_UART_RATE 9600 
 /* 串口设备句柄 */
static rt_device_t out_serial[4];                   

rt_base_t rs485_choose_pin[4][2];
rt_base_t rs485_en_pin[4][2];
 
/*
    485 通道选择
    channle 通道
    num     当前通道第几个485端口                          
    en      通道使能，RS485_ENABLE，RS485_DISABLE
    status  通道状态，读写，RS485_READ,RS485_WRITE
*/
int Choose_Rs485_Channle(unsigned char channle,unsigned char num,unsigned char en,unsigned char status)
{
    if(channle >= 4)
    {
        rt_kprintf("channle num error,channle = %d,num = %d\n",channle,num);
        return RT_ERROR;
    }
    
    if(num >= 2)
    {
        rt_kprintf("channle num error,num = %d,num = %d\n",channle,num);
        return RT_ERROR;
    }
   // rt_kprintf("channle = %d,num = %d,",channle,num);
    /* 低高平使能 */
	if(RS485_ENABLE == en)
	{
        switch(num)
        {
            case 0:
							rt_pin_write(rs485_choose_pin[channle][0],PIN_LOW);
							rt_pin_write(rs485_choose_pin[channle][1],PIN_HIGH);
            break;
            case 1:
							rt_pin_write(rs485_choose_pin[channle][0],PIN_HIGH);
							rt_pin_write(rs485_choose_pin[channle][1],PIN_LOW);
            break;    
        }
	}
	else
	{
			rt_pin_write(rs485_choose_pin[channle][num],PIN_LOW);
			//rt_kprintf("close,");
	}
	
    /* 低电平读 */
	if(RS485_READ == status)
	{
		rt_pin_write(rs485_en_pin[channle][num],PIN_LOW);
       // rt_kprintf("read.\n");
	}
	else if(RS485_WRITE == status)
	{
		rt_pin_write(rs485_en_pin[channle][num],PIN_HIGH);
       // rt_kprintf("write.\n");
	}
	else
	{
			rt_pin_write(rs485_en_pin[channle][num],PIN_LOW);
			rt_kprintf("status error,set read.\n");
	}
  return RT_EOK;
}

int Rs485_Wirte(unsigned char channle,unsigned char num,unsigned char* data,unsigned int len)
{
    int ret;
    ret = Choose_Rs485_Channle(channle,num,RS485_ENABLE,RS485_WRITE);
    if(ret != RT_EOK)
        return ret;
    ret = rt_device_write(out_serial[channle], 0, data,len);
    
    ret = Choose_Rs485_Channle(channle,num,RS485_ENABLE,RS485_READ);
    return ret;
}

UART_RX_MSG * Rs485_SetRead(unsigned char channle,unsigned char num)
{
	int ret;
	ret= Choose_Rs485_Channle(channle,num,RS485_ENABLE,RS485_READ);
	if(ret != RT_EOK)
        return NULL;
	return Rs485_GetUartStatus(channle);
}
UART_RX_MSG * Rs485_GetUartStatus(unsigned char channle)
{
		switch(channle)
		{
			case 0:
				 return getUartStatus(CHANNLE1_UART_INDEX);
			case 1:
				 return getUartStatus(CHANNLE2_UART_INDEX);
			case 2:
				 return getUartStatus(CHANNLE3_UART_INDEX);
			case 3:
				 return getUartStatus(CHANNLE4_UART_INDEX);
		}
		return NULL;
}
 

static unsigned char is485InitFlag=0;
int Init_485(void)
{
		if(is485InitFlag)
		{
			return RT_EOK;
		}
		if(Uart_Init(CHANNLE1_UART_INDEX,RS485_UART_RATE)!= RT_EOK)
				return RT_ERROR;
    if(Uart_Init(CHANNLE2_UART_INDEX,RS485_UART_RATE)!= RT_EOK)
        return RT_ERROR;
    if(Uart_Init(CHANNLE3_UART_INDEX,RS485_UART_RATE)!= RT_EOK)
        return RT_ERROR;
    if(Uart_Init(CHANNLE4_UART_INDEX,RS485_UART_RATE)!= RT_EOK)
        return RT_ERROR;
    
    out_serial[0] = getUartDevice(CHANNLE1_UART_INDEX);    
    out_serial[1] = getUartDevice(CHANNLE2_UART_INDEX);
    out_serial[2] = getUartDevice(CHANNLE3_UART_INDEX);
		out_serial[3] = getUartDevice(CHANNLE4_UART_INDEX);
		
    rs485_choose_pin[0][0] = RS485_1_CHOOSE;
    rs485_choose_pin[0][1] = RS485_2_CHOOSE;
    rs485_choose_pin[1][0] = RS485_3_CHOOSE;
    rs485_choose_pin[1][1] = RS485_4_CHOOSE;
    rs485_choose_pin[2][0] = RS485_5_CHOOSE;
    rs485_choose_pin[2][1] = RS485_6_CHOOSE;
		rs485_choose_pin[3][0] = RS485_7_CHOOSE;
    rs485_choose_pin[3][1] = RS485_8_CHOOSE;
		
    rs485_en_pin[0][0] = RS485_1_EN;
    rs485_en_pin[0][1] = RS485_2_EN;
    rs485_en_pin[1][0] = RS485_3_EN;
    rs485_en_pin[1][1] = RS485_4_EN;
		rs485_en_pin[2][0] = RS485_5_EN;
    rs485_en_pin[2][1] = RS485_6_EN;
    rs485_en_pin[3][0] = RS485_7_EN;
    rs485_en_pin[3][1] = RS485_8_EN;
		
		
    rt_pin_mode(RS485_1_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_2_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_3_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_4_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_5_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_6_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_7_CHOOSE, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_8_CHOOSE, PIN_MODE_OUTPUT);
		
    rt_pin_mode(RS485_1_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_2_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_3_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_4_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_5_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_6_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_7_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(RS485_8_EN, PIN_MODE_OUTPUT);
		
		is485InitFlag=1;
    return RT_EOK;
}

int Get_Rs485_Init_Flag(void)
{
	return is485InitFlag;
}

 void test485()
{
	static unsigned char i=0,num=0;
	Rs485_Wirte(i,num,"1231223123121231231",rt_strlen("1231223123121231231"));
	rt_thread_mdelay(1000);
}

