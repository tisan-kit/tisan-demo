/*
 * tisan_gpio_intr.c
 *
 *  Created on: 2015Äê10ÔÂ29ÈÕ
 *      Author: Administrator
 */

#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "../../pando/pando_subdevice.h"
#include "tisan_gpio_intr.h"


void gpio_intr_handler(struct base_key_param **keys_param)
{
    uint8 i;
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	uint8 gpio_id;

	struct base_key_param * single_key = NULL;

    PRINTF("Get into gpio_intr_handler, gpio_status:%u\n", gpio_status);

	for(i = 0; i < KEY_MAX_NUM; i++)
	{
		single_key = keys_param[i];

		if(single_key == NULL)
		{
			PRINTF("\r\nintr sinle_key is NULL, i:%d\r\n",i);
			continue;
		}

		gpio_id = single_key->gpio_id;
		PRINTF("\r\ngpio_id:%d, BIT(gpio_id):%d\r\n",gpio_id, BIT(gpio_id));
		if(gpio_status & BIT(gpio_id))
		{
			PRINTF("\r\n key config start....\r\n");
			//disable interrupt
			gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_id), GPIO_PIN_INTR_DISABLE);
			//clear interrupt status
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(gpio_id));

			//if need to manage special gpio event,  manage here
			//example: manage key config wifi connect, need call config_key_init() first
			if(i == 0)
			{
				if(single_key->level == 1)
				{// 5s, restart & enter softap mode
					PRINTF("\r\n sinle_key->level:%d....\r\n", single_key->level);
					os_timer_disarm(&single_key->k_timer1);
					os_timer_setfn(&single_key->k_timer1, (os_timer_func_t *)key_5s_cb,
							single_key);
					os_timer_arm(&single_key->k_timer1, LONG_PRESS_COUNT, 0);
					single_key->level = 0;
					gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_id), GPIO_PIN_INTR_POSEDGE);
				}
				else
				{
					// 50ms, check if this is a real key up
					PRINTF("\r\n50ms sinle_key->level:%d....\r\n", single_key->level);
					os_timer_disarm(&single_key->k_timer2);
					os_timer_setfn(&single_key->k_timer2, (os_timer_func_t *)key_50ms_cb,
							single_key);
					os_timer_arm(&single_key->k_timer2, 50, 0);
				}

				continue;
			}

			//other manage

		}
	}


}



