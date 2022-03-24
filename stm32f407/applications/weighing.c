//#include "weighing.h"
//#include "infrared.h"
//#include "bt.h"
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include <string.h>

//weighing_t weighing;

////void Weighting_Handle(void)
////{
////    /* 准备到位 */
////    if(Infrared_Ready() == 3)
////    {
////        /* 有二维码 */
////        if(weighing.rq_code_fresh == 1)
////        {
////            /* 读电子标签 */
////            if(Get_Uuid(weighing.uuid,&weighing.rssi) == RT_EOK )
////            {
////                /* 读取重量无误 */
////                if(Weigh_Read(&weighing.weight_value,&weighing.tare_value) == RT_EOK)
////                {
////                    /* 上报数据 */
////                    
////                }
////            }
////        }
////    }
////}



