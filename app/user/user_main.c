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
#include "../peripheral/peri_key.h"
#include "wifi_config.h"

static void ICACHE_FLASH_ATTR
user_key_long_press_cb()
{
	wifi_config(NULL);
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

	//long press gpio4, enter into wifi config mode.
	peri_single_key_init(PERIPHS_IO_MUX_GPIO4_U, user_key_long_press_cb, NULL);

	led_object_init();

	pando_gateway_init();
}

