//#include "rq_code_read.h"
//#include <rtthread.h>
//#include <rtdbg.h>
//#include <board.h>
//#include "rq_code_rec.h"
//#include <string.h>
//#include "weighing.h"
//#include "main_task.h"
////extern weighing_t weighing;
//unsigned char rqcode_rec_buf[MAX_RQ_CODE_BUF];
//static void rqcode_read_thread(void *param)
//{
//    unsigned int len = 0;
//    unsigned int ret = 0;
//    if(Rqcode_Init() == RT_ERROR)
//    {
//        while(1)
//        {
//            rt_kprintf("[rqcode]init error\n");
//            rt_thread_delay(10000);
//        }
//    }
//        
//    while(1)
//    {
//        memset(rqcode_rec_buf,0,MAX_RQ_CODE_BUF);
//        ret = Rqcode_Read(rqcode_rec_buf,&len);
//        if(ret == RT_EOK)
//        {
//            if((len > 0)&&(weighing.rq_code_fresh==0))
//            {
//                /* 将读取内容填充到共享内存 */
//                memcpy(weighing.rq_code_buf,rqcode_rec_buf,MAX_RQ_CODE_BUF);
//								weighing.rq_code_fresh=1;
//                //rt_memcpy(systemStatus.rqCode,rqcode_rec_buf,MAX_RQ_CODE_BUF);
//                rt_kprintf("[rqcode]%s,len = %d\n",rqcode_rec_buf,len);
//            }
//        }
//        rt_thread_delay(30);
//    }

// }
// 
// 
//int rqcode_read_start(void)
//{
//    rt_thread_t tid = RT_NULL;
//    
//    tid = rt_thread_create( "rqcode",
//                            rqcode_read_thread, RT_NULL,      // fun, parameter
//                            512,          // stack size
//                            9, 10 );         //priority, tick      
//    if (tid)
//    {
//        rt_thread_startup(tid);
//    }

//    return RT_EOK;
//}





