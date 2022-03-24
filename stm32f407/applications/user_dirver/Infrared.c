#include "infrared.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "system_params.h"
#include "main_task.h"
#define RED1_IN1 GET_PIN(A, 0)      //IN1
#define RED1_IN2 GET_PIN(A, 3)      //IN2
#define RED1_IN3 GET_PIN(A, 4)      //IN3

#define RED2_IN1 GET_PIN(A, 5)      //IN4
#define RED2_IN2 GET_PIN(A, 6)      //IN5
#define RED2_IN3 GET_PIN(B, 0)      //IN6

void Init_Infrared(void)
{
    rt_pin_mode(RED1_IN1, PIN_MODE_INPUT);
    rt_pin_mode(RED1_IN2, PIN_MODE_INPUT);
    rt_pin_mode(RED1_IN3, PIN_MODE_INPUT);
    
    rt_pin_mode(RED2_IN1, PIN_MODE_INPUT);
    rt_pin_mode(RED2_IN2, PIN_MODE_INPUT);
    rt_pin_mode(RED2_IN3, PIN_MODE_INPUT);
}
 
unsigned char cheackInfrared(void)
{
	unsigned char i=0;
	 if((systemStatus.redCnt>=1)&&(rt_pin_read(RED1_IN1)) != 0)
	 {
			i|=0x04;
	 }
	  if((systemStatus.redCnt>=2)&&(rt_pin_read(RED1_IN2) != 0))
	 {
		  if(systemStatus.redCnt==2)
			{
				i|=0x01;
			}
			else//systemStatus.redCnt=4
			{
				i|=0x04;
			}
	 }
 
	 if((systemStatus.redCnt>=3)&&(rt_pin_read(RED1_IN3) != 0))
	 {
			i|=0x01;
	 }
	 if((systemStatus.redCnt>=4)&&(rt_pin_read(RED2_IN1) != 0))
	 {
			i|=0x01;
	 }
	 return i;
}
 