//#include "uart1_use.h"
//#include <string.h>

//#define SAMPLE_UART1_NAME    "uart1"

//rt_device_t serial1;
//unsigned char usart1_init_ok;

///* ���ڽ�����Ϣ�ṹ*/
//struct rx_msg
//{
//    rt_device_t dev;
//    rt_size_t size;
//};

///* ��Ϣ���п��ƿ� */
//static struct rt_messagequeue rx_mq;
//static struct rx_msg msg;
//static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
//static char msg_pool[256];
///* �������ݻص����� */
//static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
//{
//    struct rx_msg msg;
//    rt_err_t result;
//    msg.dev = dev;
//    msg.size = size;

//    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
//    if ( result == -RT_EFULL)
//    {
//        /* ��Ϣ������ */
//      //  rt_kprintf("message queue full��\n");
//    }
//    return result;
//}

//int Uart1_Wirte(unsigned char* data,unsigned int len)
//{
//    if(serial1 == NULL)
//        return RT_ERROR;
//    
//    return rt_device_write(serial1, 0, data,len);
//}

//int Uart1_Read(unsigned char* data,unsigned int *len)
//{
//    if(serial1 == RT_NULL)
//    {
//        return RT_ERROR;
//    }
//    
//    rt_err_t result;
//    
//    /* �����Ϣ */
//    result = rt_mq_recv(&rx_mq, &msg, sizeof(msg), 50);
//    if (result == RT_EOK)
//    {
//        /* �Ӵ��ڶ�ȡ����*/
//        *len = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
//        
//        memcpy(data,rx_buffer,*len);
//        
//        /* ��ӡ���� */
//        //rt_kprintf("%s\n",rx_buffer);
//    }
//    
//    return result;
//}


//int Uart1_Init(unsigned int rate)
//{
//    if(usart1_init_ok == 1)
//        return RT_EOK;
//    int ret = 0;
//    /* ���Ҵ����豸 */
//    serial1 = rt_device_find(SAMPLE_UART1_NAME);
//    
//    if(serial1 == RT_NULL)
//    {
//        return RT_ERROR;
//    }
//    
//    /* step2���޸Ĵ������ò��� */
//    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
//    config.baud_rate = rate;        //�޸Ĳ�����Ϊ 9600
//    config.data_bits = DATA_BITS_8;           //����λ 8
//    config.stop_bits = STOP_BITS_1;           //ֹͣλ 1
//    config.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
//    config.parity    = PARITY_NONE;           //����żУ��λ

//    /* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
//    rt_device_control(serial1, RT_DEVICE_CTRL_CONFIG, &config);
//    
//    /* ���豸 */
//    ret = rt_device_open(serial1,RT_DEVICE_FLAG_DMA_RX);
//    
//    if(ret != RT_EOK)
//        return ret;
//    
//    /* ���ý��ջص����� */
//    rt_mq_init(&rx_mq, "rx_mq1",
//               msg_pool,                 /* �����Ϣ�Ļ����� */
//               sizeof(struct rx_msg),    /* һ����Ϣ����󳤶� */
//               sizeof(msg_pool),         /* �����Ϣ�Ļ�������С */
//               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
//    rt_device_set_rx_indicate(serial1, uart_input);
//    
//    
//    //Uart1_Wirte((unsigned char *)"uart1 test\n",sizeof("uart1 test\n")-1);
//    usart1_init_ok = 1;
//    return RT_EOK;
//}
