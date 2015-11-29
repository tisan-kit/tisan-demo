/*******************************************************
 * File name: peri_key.c
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This is the key function module.

 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/

#include "peri_key.h"
#include "user_interface.h"
#include "eagle_soc.h"
#include "driver/key.h"
#include "os_type.h"
#include "mem.h"
#include "driver/tisan_gpio_intr.h"
#include "../user/wifi_config.h"
 

/******************************************************************************
 * FunctionName : user_plug_short_press
 * Description  : key's short press function, needed to be installed
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_key_short_press(void)
{
	  PRINTF("short\n");
}

/******************************************************************************
 * FunctionName : user_plug_long_press
 * Description  : key's long press function, needed to be installed, preserved function.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_key_long_press(void)
{
	  PRINTF("long press..\n");
	  wifi_config(NULL);
}

void ICACHE_FLASH_ATTR
peri_config_key_init(uint8 gpio_id)
{
	struct base_key_param *key = (struct base_key_param *)os_zalloc(sizeof(struct base_key_param));

	key->gpio_id = gpio_id;
	key->gpio_name = tisan_get_gpio_name(gpio_id);
	key->gpio_func = tisan_get_gpio_general_func(gpio_id);
	key->level = 1;
	key->k_function1 = peri_key_long_press;
	key->k_function2 = peri_key_short_press;

	PRINTF("\r\nkey->gpio_id:%d\r\n", key->gpio_id);
	key_single_init(key);
}

