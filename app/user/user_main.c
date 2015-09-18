/******************************************************************************
 /*******************************************************
 * File name: user_main.c
 * Author:Chongguang Li
 * Versions:1.0
 * Description:entry file of user application.
 *
 * History:
 *   1.Date:
 *     Author:
 *     Modification:
*******************************************************************************/

#include "../peripheral/driver/uart.h"
#include "objects/led.h"
#include "os_type.h"
#include "user_interface.h"

static os_timer_t wifi_check_timer;

/******************************************************************************
 * FunctionName : wifi_connect_check
 * Description  : check the wifi connect status. if device has connected the wifi,
 * 				  start the gateway flow.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
wifi_connect_check()
{
	struct ip_info device_ip;
	uint8 connect_status = 0;
	wifi_get_ip_info(STATION_IF, &device_ip);
	connect_status = wifi_station_get_connect_status();
	if (connect_status == STATION_GOT_IP && device_ip.ip.addr != 0) // device has connected the wifi.
	{
		os_timer_disarm(&wifi_check_timer);
		pando_gateway_init();
	}
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
	uart_init(115200, 115200); // serial bound rate:11520.

	led_object_init();

	// check the wifi connect status.
	os_timer_disarm(&wifi_check_timer);
	os_timer_setfn(&wifi_check_timer, (os_timer_func_t *)wifi_connect_check, NULL);
	os_timer_arm(&wifi_check_timer, 3000, 1);
}

