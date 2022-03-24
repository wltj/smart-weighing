//#include "weigh_thread.h"
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include "weigh.h"
//#include "main_task.h"
//#include "weighing.h"
//#include "printer.h"
//#include "uart5_use.h"

// 
//unsigned char old_used;
//unsigned char now_used = WEIGH_USER;

//void changeNowUsed(unsigned char user)
//{
//	now_used=user;
//}
//static void weigh_read_thread(void *param)
//{
//    unsigned int len = 0;
//    unsigned int ret = 0;
//    //Uart5_mq_Init();
//    if(Weigh_Init() == RT_ERROR)
//		{
//				while(1)
//				{
//						rt_kprintf("[weigh]init error\n");
//						rt_thread_delay(10000);
//				}
//		}
//    while(1)
//    {
//        /*if(old_used != now_used)
//        {
//            if(now_used == WEIGH_USER)
//            {
//                Close_Uart5();
//                if(Weigh_Init() == RT_ERROR)
//                {
//                    while(1)
//                    {
//                        rt_kprintf("[weigh]init error\n");
//                        rt_thread_delay(10000);
//                    }
//                }
//            }
//            else if(now_used == PRINT_USER)
//            {
//                Close_Uart5();
//                if(Print_Init() == RT_ERROR)
//                {
//                     while(1)
//                    {
//                        rt_kprintf("[Printer]init error\n");
//                        rt_thread_delay(10000);
//                    }
//                }
//            }
//            old_used = now_used;
//        }
//        
//        if(now_used == WEIGH_USER)
//        {
//            if(getUsart5Flag() == 1)
//            {
//                double weight_value;//15.2KG
//                double tare_value;	
//                if(Weigh_Read(&weight_value,&tare_value) == RT_EOK)					//systemStatus.devStatus.speakerStatus=SPK_WORK4;
//                {
//                    weighing.weight_value = weight_value;
//                    weighing.tare_value = tare_value;
//                    weighing.weigh_fresh = 1;
//                    //rt_kprintf("[weigh]weigh = %d\n",(int)weighing.weight_value);
//                }
//            }
//        }*/
//				
//					double weight_value;//15.2KG
//					double tare_value;	
//				  //if(getUsart5Flag() == 1)
//				 {
//						if(Weigh_Read(&weight_value,&tare_value) == RT_EOK)					//systemStatus.devStatus.speakerStatus=SPK_WORK4;
//						{
//							weighing.weight_value = weight_value;
//							weighing.tare_value = tare_value;
//							weighing.weigh_fresh = 1;
//							//rt_kprintf("[weigh]weigh = %d\n",(int)weighing.weight_value);
//						}
//					}
//					rt_thread_delay(10);
//    }

// }

//void Close_Weigh(void)
//{
//  //  Close_Uart5();
//}

//void Open_Weigh(void)
//{
// //   Open_Uart5();
//}
// 
//int weigh_read_start(void)
//{
//    rt_thread_t tid = RT_NULL;
//    
//    tid = rt_thread_create( "weigh",
//                            weigh_read_thread, RT_NULL,      // fun, parameter
//                            2048,          // stack size
//                            7, 10 );         //priority, tick      
//    if (tid)
//    {
//        rt_thread_startup(tid);
//    }

//    return RT_EOK;
//}