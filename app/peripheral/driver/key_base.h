/*
 * key_base.h
 *
 *  Created on: 2015Äê11ÔÂ6ÈÕ
 *      Author: wfq
 */

#ifndef APP_PERIPHERAL_DRIVER_KEY_BASE_H_
#define APP_PERIPHERAL_DRIVER_KEY_BASE_H_

#include "gpio.h"
#include "os_type.h"

#define KEY_MAX_NUM  3

typedef void (* key_base_function)(void);

struct base_key_param{
	uint8 level;
	uint8 gpio_id;
	uint8 gpio_func;
	uint32 gpio_name;
	uint32 counter;
	os_timer_t k_timer1;
	os_timer_t k_timer2;
	key_base_function k_function1;
	key_base_function k_function2;
};

//corresponding driver file will call key_single_init function
void key_single_init(struct base_key_param * single_key_param);
//base_keys_init call at last(after all driver needed has init)
void base_keys_init(void);


#endif /* APP_PERIPHERAL_DRIVER_KEY_BASE_H_ */
