#include "traffic_light.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
/*   ¹ýµÀ¸Ë   */
#define RED_PIN	 		 GET_PIN(E, 0)
#define GREEN_PIN 	 GET_PIN(E, 1)
#define WARNING_PIN  GET_PIN(B, 10)

#define LEDRELAY_ON					PIN_HIGH
#define LEDRELAY_OFF			(!LEDRELAY_ON)
#define DELAY_TIME			500
void trafficLightInit(void)
{
		rt_pin_mode(RED_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(GREEN_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(WARNING_PIN, PIN_MODE_OUTPUT);
		rt_thread_mdelay(DELAY_TIME);
		showProhibit();
}
void showProhibit(void)
{
		rt_pin_write(GREEN_PIN, LEDRELAY_OFF);
		rt_thread_mdelay(DELAY_TIME);
		rt_pin_write(RED_PIN, LEDRELAY_ON);
		//-----------------------------------
		rt_pin_write(WARNING_PIN, LEDRELAY_ON);
}
void showPass(void)
{
		rt_pin_write(RED_PIN, LEDRELAY_OFF);
		rt_thread_mdelay(DELAY_TIME);
		rt_pin_write(GREEN_PIN, LEDRELAY_ON);
		rt_pin_write(WARNING_PIN, LEDRELAY_OFF);
}
