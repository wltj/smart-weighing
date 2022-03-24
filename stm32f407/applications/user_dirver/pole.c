#include "pole.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
/*   过道杆   */
#define POLE_OPEN_PIN	 GET_PIN(B, 7)
#define POLE_CLOSE_PIN GET_PIN(B, 8)
#define POLE_STOP_PIN  GET_PIN(B, 9)

#define PORELAY_ON			PIN_HIGH
#define PORELAY_OFF			(!PORELAY_ON)

#define DELAY_TIME			3000

void poleInit(void)
{
	  rt_pin_mode(POLE_OPEN_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(POLE_CLOSE_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(POLE_STOP_PIN, PIN_MODE_OUTPUT);
		//------------------------------------------
		rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
		rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
		rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
		rt_thread_mdelay(DELAY_TIME);
		openRoadGate();
		//TEST------------------------
//		while(1)
//		{
//			
////		rt_pin_write(POLE_STOP_PIN, PORELAY_ON);
////		rt_thread_mdelay(5000);
////		rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
////		rt_thread_mdelay(5000);
//			rt_pin_write(POLE_OPEN_PIN, PORELAY_ON);
//			rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
//			rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
//			rt_thread_mdelay(1000);
//			rt_pin_write(POLE_OPEN_PIN, PORELAY_OFF);
//			rt_pin_write(POLE_CLOSE_PIN, PORELAY_ON);
//			rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
//			rt_thread_mdelay(1000);
//			rt_pin_write(POLE_OPEN_PIN, PORELAY_OFF);
//			rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
//			rt_pin_write(POLE_STOP_PIN, PORELAY_ON);
//			rt_thread_mdelay(1000);
//		}
}

//打开过道杆
void openRoadGate(void)
{
	rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
	rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
	rt_pin_write(POLE_OPEN_PIN, PORELAY_ON);
	rt_thread_mdelay(DELAY_TIME);
	rt_pin_write(POLE_OPEN_PIN, PORELAY_OFF);
	rt_kprintf("------Open road gate-------------\r\n");
}
//关闭过道杆
void closeRoadGate(void)
{
	rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
	rt_pin_write(POLE_OPEN_PIN, PORELAY_OFF);
	rt_pin_write(POLE_CLOSE_PIN, PORELAY_ON);
	rt_thread_mdelay(DELAY_TIME);

	rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
	rt_kprintf("------close road gate-------------\r\n");
}
void stopRoadGate(void)
{
	rt_pin_write(POLE_CLOSE_PIN, PORELAY_OFF);
	rt_pin_write(POLE_OPEN_PIN, PORELAY_OFF);
	rt_pin_write(POLE_STOP_PIN, PORELAY_ON);
	rt_thread_mdelay(DELAY_TIME);
	rt_pin_write(POLE_STOP_PIN, PORELAY_OFF);
	rt_kprintf("------stop road gate-------------\r\n");
}


