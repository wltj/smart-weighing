#include "mqtt_thread.h"
#include "main.h"
#include <rtthread.h>
#include "cmd_handle.h"
#include "com_to_server_cmd_list.h"
#include "gateway.h"
#include "com_to_server_thread.h"


#define DBG_ENABLE
#define DBG_SECTION_NAME    "mqttcmd"
#define DBG_LEVEL           DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
#define REC_SERVER_DATA_LEN 1000
#define SEND_SERVER_DATA_LEN  1000

#define GATE_FW 0
#define TERM_FW 1

unsigned char rec_server_data[1000];
unsigned char send_server_data[1000];
/*
    网关注册
*/
int Gateway_Register_To_Server(void)
{
//    Gateway_Register Gateway_Register_Data;
//    memset((unsigned char *)&Gateway_Register_Data,0,sizeof(Gateway_Register));
//    memcpy((unsigned char *)Gateway_Register_Data.Mac_Id,get_macid_string(),MACID_STRING_LENTH);
//    Gateway_Register_Data.Unit_Num = gateway.unit_num;//单元号
//    Gateway_Register_Data.Unit_T = gateway.unit_type; //单元类型
//    Gateway_Register_Data.Unit_Term_Num = gateway.term_num; //单元终端数
//    Gateway_Register_Data.Wireless_Params.Wireless_Channel = gateway.wireless_param.wireless_channle; //无线信道
//    Gateway_Register_Data.Wireless_Params.Wireless_Netid = gateway.wireless_param.netid;
//    memset(send_server_data,0,SEND_SERVER_DATA_LEN);
//    /* Create a stream that will write to our buffer. */
//    pb_ostream_t stream = pb_ostream_from_buffer(send_server_data, SEND_SERVER_DATA_LEN);
//    
//    /* Now we are ready to encode the message! */
//    if(pb_encode(&stream, Gateway_Register_fields, &Gateway_Register_Data) == true)
//    {
//        cmd_send_handle(MQTT_REG_TOPIC,Cmd_Type__CMD_GATEWAY_REGISTER,send_server_data,stream.bytes_written);
//       
//        LOG_D(" gateway register request send ok ! ");
//        return RT_EOK;
//    }
//    LOG_D(" error :gateway register request send failed ! ");

    return RT_ERROR;
}


int Common_Ack_Decode(unsigned char *pb,unsigned int len)
{
//    Return_Common_Msg com_msg;
//    memset(&com_msg,0,sizeof(Return_Common_Msg));

//    pb_istream_t stream = pb_istream_from_buffer(pb, len);
//    
//    if(pb_decode(&stream, Return_Common_Msg_fields, &com_msg))
//    {
//        LOG_D("return msg %s ! ",com_msg.Error_Msg);
//        return com_msg.Errorcode;
//    }
    return RT_ERROR;
}







