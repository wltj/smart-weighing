//#include "bt_thread.h"
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include "bt.h"
//#include "main_task.h"
//#include <string.h>
//#include "weighing.h"



//static void bt_read_thread(void *param)
//{
//    Bt_Init();
//        
//    while(1)
//    {
//        if(Get_Uuid(weighing.uuid,&weighing.rssi) == RT_EOK )
//        {
//            weighing.bt_fresh = 1;
//        }
//        rt_thread_delay(10);
//    }

// }
// 
// 
//int bt_read_start(void)
//{
//    rt_thread_t tid = RT_NULL;
//    
//    tid = rt_thread_create( "bt",
//                            bt_read_thread, RT_NULL,      // fun, parameter
//                            2048,          // stack size
//                            9, 10 );         //priority, tick      
//    if (tid)
//    {
//        rt_thread_startup(tid);
//    }

//    return RT_EOK;
//}


