//#include "uart4_use.h"
//#include <string.h>


//#define SAMPLE_UART4_NAME    "uart4"

//rt_device_t serial4;
//static unsigned char usart4_flag = 0;

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
//static int rec_size = 0;
//static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
//{
//    rt_err_t result = RT_EOK;
//    if(usart4_flag == 1)
//    {
//        struct rx_msg msg;
//        
//        msg.dev = dev;
//        
//        rec_size += size;
//        msg.size = rec_size;
//        if(rec_size >= 23)
//        {
//            rec_size = 0;
//            result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
//        }
//        if ( result == -RT_EFULL)
//        {
//            /* ��Ϣ������ */
//           // rt_kprintf("message queue full!\n");
//        }
//    }
//    return result;
//}


//int Uart4_Wirte(unsigned char* data,unsigned int len)
//{
//    if(serial4 == NULL)
//        return RT_ERROR;
//    
//    return rt_device_write(serial4, 0, data,len);
//}

//int Uart4_Read(unsigned char* data,unsigned int *len)
//{
//    if(serial4 == RT_NULL)
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


//int Uart4_Init(unsigned int rate)
//{
//    int ret = 0;
//    /* ���Ҵ����豸 */
//    serial4 = rt_device_find(SAMPLE_UART4_NAME);
//    
//    if(serial4 == RT_NULL)
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
//    rt_device_control(serial4, RT_DEVICE_CTRL_CONFIG, &config);
//    
//    /* ���豸 */
//    ret = rt_device_open(serial4,RT_DEVICE_FLAG_DMA_RX);
//    
//    if(ret != RT_EOK)
//        return ret;
//    
//    /* ���ý��ջص����� */
//    rt_mq_init(&rx_mq, "rx_mq4",
//               msg_pool,                 /* �����Ϣ�Ļ����� */
//               sizeof(struct rx_msg),    /* һ����Ϣ����󳤶� */
//               sizeof(msg_pool),         /* �����Ϣ�Ļ�������С */
//               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
//    rt_device_set_rx_indicate(serial4, uart_input);
//       
//    //Uart4_Wirte((unsigned char *)"uart4 test\n",sizeof("uart1 test\n")-1);
//    
//    return RT_EOK;
//}

//void Close_Uart4(void)
//{
//    usart4_flag = 0;
//}

//void Open_Uart4(void)
//{
//    usart4_flag = 1;
//}
