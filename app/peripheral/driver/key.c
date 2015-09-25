/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: key.c
 *
 * Description: key driver, now can use different gpio and install different function
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "../peri_key.h"

#include "key.h"

#define LONG_PRESS_COUNT 3000
/******************************************************************************
 * FunctionName : key_init
 * Description  : init keys
 * Parameters   : key_param *keys - keys parameter, which inited by key_init_single
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
key_init(uint32 gpio_name,uint8 gpio_id,uint8 gpio_func)
{
        PIN_FUNC_SELECT(gpio_name,gpio_func);
        PIN_PULLUP_EN(gpio_name);
        gpio_output_set(0, 0, 0, GPIO_ID_PIN(gpio_id));

        gpio_register_set(GPIO_PIN_ADDR(gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
                          | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
                          | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

        //clear interrupt status
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(gpio_id));

        //enable interrupt
        gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_id), GPIO_PIN_INTR_NEGEDGE);
}
/******************************************************************************
 * FunctionName : key_5s_cb
 * Description  : long press 5s timer callback
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
key_5s_cb(struct key_param *single_key)
{
    os_timer_disarm(&single_key->key_5s);

    if (0 == GPIO_INPUT_GET(GPIO_ID_PIN(single_key->gpio_id))) {
        if (single_key->long_press) {
            single_key->long_press();
        }
    }
}

/******************************************************************************
 * FunctionName : key_50ms_cb
 * Description  : 50ms timer callback to check it's a real key push
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
key_50ms_cb(struct key_param *single_key)
{
    os_timer_disarm(&single_key->key_50ms);

    // high, then key is up
    if (1 == GPIO_INPUT_GET(GPIO_ID_PIN(single_key->gpio_id))) {
        os_timer_disarm(&single_key->key_5s);
        single_key->key_level = 1;
        gpio_pin_intr_state_set(GPIO_ID_PIN(single_key->gpio_id), GPIO_PIN_INTR_NEGEDGE);

        if (single_key->short_press) {
            single_key->short_press();
        }
    } else {
        gpio_pin_intr_state_set(GPIO_ID_PIN(single_key->gpio_id), GPIO_PIN_INTR_POSEDGE);
    }
}

/******************************************************************************
 * FunctionName : key_intr_handler
 * Description  : key interrupt handler
 * Parameters   : key_param *keys - keys parameter, which inited by key_init_single
 * Returns      : none
*******************************************************************************/
void key_intr_handler(struct key_param *key)
{
    uint8 i;
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

        if (gpio_status & BIT(key->gpio_id))
        {
            //disable interrupt
            gpio_pin_intr_state_set(GPIO_ID_PIN(key->gpio_id), GPIO_PIN_INTR_DISABLE);

            //clear interrupt status
            GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(key->gpio_id));

            if (key->key_level == 1)
            {
                // 5s, restart & enter softap mode
                os_timer_disarm(&key->key_5s);
                os_timer_setfn(&key->key_5s, (os_timer_func_t *)key_5s_cb, key);
                os_timer_arm(&key->key_5s, LONG_PRESS_COUNT, 0);
                key->key_level = 0;
                gpio_pin_intr_state_set(GPIO_ID_PIN(key->gpio_id), GPIO_PIN_INTR_POSEDGE);
            }
            else
            {
                // 50ms, check if this is a real key up
                os_timer_disarm(&key->key_50ms);
                os_timer_setfn(&key->key_50ms, (os_timer_func_t *)key_50ms_cb, key);
                os_timer_arm(&key->key_50ms, 50, 0);
            }
        }

}

