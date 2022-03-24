//#include "uart5_use.h"
//#include <string.h>

//#define SAMPLE_UART5_NAME    "uart5"

//rt_device_t serial5;
//unsigned char usart5_flag = 0;

///* 串口接收消息结构*/
//struct rx_msg
//{
//    rt_device_t dev;
//    rt_size_t size;
//};

///* 消息队列控制块 */
//static struct rt_messagequeue rx_mq5;
//static struct rx_msg msg; 
//static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
//static char msg_pool[1024];
// 
//static int index=0;

///* 接收数据回调函数 */
//static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
//{
//		unsigned char buff[512]={0};
//    rt_err_t result = RT_EOK;
//		int  len=0;
//		len=rt_device_read(dev, 0, buff, size);
//		if(usart5_flag == 1)
//		{
//			if((index+len)<=RT_SERIAL_RB_BUFSZ)
//			{
//				rt_memcpy(rx_buffer+index,buff,len);
//				index+=len;
//			}
//			else{
//				rt_memcpy(rx_buffer,buff,len);
//				index=len;
//			}
//		}
//		else
//		{
//			index=0;
//		}
////    if(usart5_flag == 1)
////    {
////        struct rx_msg msg;
////        
////        msg.dev = dev;
////        msg.size = size;
////				
////        result = rt_mq_send(&rx_mq5, &msg, sizeof(msg));
////        if ( result == -RT_EFULL)
////        {
////            /* 消息队列满 */
////           // rt_kprintf("message queue full！\n");
////        }
////    } 
//    return result;
//}


//int Uart5_Wirte(unsigned char* data,unsigned int len)
//{
//    if(serial5 == NULL)
//        return RT_ERROR;
//    
//    return rt_device_write(serial5, 0, data,len);
//}
//int Uart5_Read(unsigned char* data,unsigned int* len)
//{
//		if(index<=30)
//		{
//			rt_memcpy(data,rx_buffer,index);
//			*len=index;
//		}
//		else
//		{
//			rt_memcpy(data,rx_buffer+index-30,30);
//			*len=30;
//		}
//		index=0;
//		return RT_EOK;
//}
////int Uart5_Read(unsigned char* data,unsigned int* len)
////{
////    if(serial5 == RT_NULL)
////    {
////        return RT_ERROR;
////    }
////    
////    rt_err_t result;
////  
////    /* 检测消息 */
////    result = rt_mq_recv(&rx_mq5, &msg, sizeof(msg), 50);
////    if (result == RT_EOK)
////    {
////        /* 从串口读取数据*/
////        *len = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
////        
////        memcpy(data,rx_buffer,*len);
////        
////        /* 打印数据 */
////        //rt_kprintf("%s\n",rx_buffer);
////    }
////    
////    return result;
////}

//void Uart5_mq_Init(void)
//{
//    rt_mq_init(&rx_mq5, "rx_mq5",
//               msg_pool,                 /* 存放消息的缓冲区 */
//               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
//               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
//               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
//}


//int Uart5_Init(unsigned int rate)
//{
//    int ret = 0;
//    /* 查找串口设备 */
//    serial5 = rt_device_find(SAMPLE_UART5_NAME);
//    
//    if(serial5 == RT_NULL)
//    {
//        return RT_ERROR;
//    }
//    
//    /* step2：修改串口配置参数 */
//    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
//    config.baud_rate = rate;        //修改波特率为 9600
//    config.data_bits = DATA_BITS_8;           //数据位 8
//    config.stop_bits = STOP_BITS_1;           //停止位 1
//    config.bufsz     = 256;                   //修改缓冲区 buff size 为 128
//    config.parity    = PARITY_NONE;           //无奇偶校验位

//    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
//    rt_device_control(serial5, RT_DEVICE_CTRL_CONFIG, &config);
//    
//    /* 打开设备 */
//    ret = rt_device_open(serial5,RT_DEVICE_FLAG_DMA_RX);
//    
//    if(ret != RT_EOK)
//        return ret;
//    
//    /* 设置接收回调函数 */
//    rt_device_set_rx_indicate(serial5, uart_input);
//    
//  //  rt_device_set_rx_indicate(serial5, uart_input);

//    return RT_EOK;
//}


//void Close_Uart5(void)
//{
//    usart5_flag = 0; 
//}
//int getUsart5Flag(void)
//{
//	return usart5_flag;
//}
//int Open_Uart5(void)
//{
//    usart5_flag = 1; 
//		index=0;
//		return usart5_flag;
//}
