///*
// * @Description: 牧原集团
// * @Version: V1.0.0
// * @Author: YuXianZe
// * @Date: 2020-03-23 00:07:11
// * @LastEditors: YuXianze
// * @LastEditTime: 2020-04-12 22:24:17
// * @FilePath: \stm32f407\applications\gateway.c
// */
//#include "gateway.h"
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include "fal.h"
//#include "easyflash.h"
//#include "sys.h"


////gateway_t gateway;
////void Init_Gateway_Param(void)
////{
////    if (!ef_get_env_blob("gateway", &gateway, sizeof(gateway_t), RT_NULL))
////    {
////        /* 初始化gateway */
//////        gateway.fw = APP_FW;
//////        gateway.hw = BOARD_HW;

//////        gateway.gateway_type = Gateway_Type_STM32F407;
//////        gateway.term_num = TERM_NUM;
//////        gateway.unit_type = Unit_Type_UNIT_NO_TYPE;
//////        gateway.unit_num = 0;
//////        get_mac_addr(gateway.macid);
//////        gateway.wireless_param.netid = (unsigned short)gateway.macid[0] + gateway.macid[1] + gateway.macid[2] + gateway.macid[3] + gateway.macid[4] + gateway.macid[5];
//////        gateway.wireless_param.wireless_channle = 0;
//////        gateway.wireless_power = 10;
//////        Save_Gateway_Param();
////    }
////	Display_Gatewat_Param();
////}

////void Get_Gateway_Param(void)
////{
////    ef_get_env_blob("gateway", &gateway, sizeof(gateway_t), RT_NULL);
////}

////void Save_Gateway_Param(void)
////{
////    ef_set_env_blob("gateway", &gateway, sizeof(gateway_t));
////}

////void Display_Gatewat_Param(void)
////{
//////    //读取Gateway-Param
//////    Get_Gateway_Param();
//////    //打印param
//////    rt_kprintf("[gateway]--Param\n");
//////    rt_kprintf("[gateway].Mac ID = %d.%d.%d.%d.%d.%d\n", gateway.macid[0],gateway.macid[1],gateway.macid[2],gateway.macid[3],gateway.macid[4],gateway.macid[5]);

//////    rt_kprintf("[gateway].IP = %d.%d.%d.%d\n", gateway.ip[0], gateway.ip[1],gateway.ip[2], gateway.ip[3]);
//////    rt_kprintf("[gateway].fw_Version = %d\n", gateway.fw);
//////    rt_kprintf("[gateway].hw_Version = %d\n", gateway.hw);
//////    if (gateway.gateway_type == Gateway_Type_STM32F407)
//////    {
//////        rt_kprintf("[gateway].Gateway Type : Gateway_Type_STM32F407\n");
//////    }
//////    else if (gateway.gateway_type == Gateway_Type_GATEWAY_NO_TYPE)
//////    {
//////        rt_kprintf("[gateway].Gateway Type : Gateway_Type_GATEWAY_NO_TYPE\n");
//////    }
//////    else
//////    {
//////        rt_kprintf("[gateway].Gateway Type : No this type ,data=%d\n", gateway.gateway_type);
//////    }

//////    rt_kprintf("[gateway].term Can Have : %d\n", gateway.term_num);
//////    if (gateway.unit_type == Unit_Type_UNIT_NO_TYPE)
//////    {
//////        rt_kprintf("[gateway].Unit Type : Gateway_Type_STM32F407\n");
//////    }
//////    else if (gateway.unit_type == Unit_Type_LACTATING)
//////    {
//////        rt_kprintf("[gateway].Unit Type : Gateway_Type_GATEWAY_NO_TYPE\n");
//////    }
//////    else if (gateway.unit_type == Unit_Type_GESTATING)
//////    {
//////        rt_kprintf("[gateway].Unit Type : Gateway_Type_GATEWAY_NO_TYPE\n");
//////    }
//////    else
//////    {
//////        rt_kprintf("[gateway].Unit Type : No this type ,data=%d\n", gateway.unit_type);
//////    }
//////    rt_kprintf("[gateway].Unit Number : %d\n", gateway.unit_num);
//////    rt_kprintf("[gateway].Gateway Seq : %d\n", gateway.seq);
//////    rt_kprintf("[gateway].Gateway Wireless Paramter:\n");
//////    rt_kprintf("[gateway].Net ID : %d\n", gateway.wireless_param.netid);
//////    rt_kprintf("[gateway].Channle ID : %d\n", gateway.wireless_param.wireless_channle);
////    // rt_kprintf("    lora_status : %d\n", gateway.lora_status);
////}
////MSH_CMD_EXPORT(Display_Gatewat_Param, Display Gateway Paramter);
