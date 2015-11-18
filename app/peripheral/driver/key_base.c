/*
 * key_base.c
 *
 *  Created on: 2015Äê11ÔÂ6ÈÕ
 *      Author: Wfq
 */
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "key_base.h"

#include "tisan_gpio_intr.h"

struct base_key_param *keys_param[KEY_MAX_NUM];
static uint8 key_offset = 0;


void ICACHE_FLASH_ATTR 
key_single_init(struct base_key_param * single_key_param)
{
//	static uint8 offset = 0;

	if(key_offset >= KEY_MAX_NUM)
		return;
	
	keys_param[key_offset++] = single_key_param;
}

void ICACHE_FLASH_ATTR 
base_keys_init(void)
{
	uint8 i;
	struct base_key_param * skey = NULL;

	PRINTF("\r\nbase_keys_init....\r\n");
	ETS_GPIO_INTR_ATTACH(gpio_intr_handler, keys_param);

	ETS_GPIO_INTR_DISABLE();

	for(i = 0; i < KEY_MAX_NUM; i++)
	{
		skey = keys_param[i];
		if(skey == NULL)
		{
			PRINTF("\r\nskey null, i:%d\r\n",i);
			break;
		}
		PRINTF("\r\ninit gpio_id:%d ..\r\n", skey->gpio_id);

		//example:config key ,  need call peri_config_key_init() first
		//example: vibrate key, need call peri_vibrate_init() second
		if((i == 0) || (i == 1))  //
		{
			skey->level = 1;		
			PIN_FUNC_SELECT(tisan_get_gpio_name(skey->gpio_id), 
				tisan_get_gpio_general_func(skey->gpio_id));
			PIN_PULLUP_EN(tisan_get_gpio_name(skey->gpio_id));
			gpio_output_set(0, 0, 0, GPIO_ID_PIN(skey->gpio_id));
			gpio_register_set(GPIO_PIN_ADDR(skey->gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
				| GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
				| GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

			//clear gpio status
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(skey->gpio_id));

			//enable interrupt
			gpio_pin_intr_state_set(GPIO_ID_PIN(skey->gpio_id), 
				GPIO_PIN_INTR_NEGEDGE);
		}
	}

	ETS_GPIO_INTR_ENABLE();
}




