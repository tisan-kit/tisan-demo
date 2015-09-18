#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"
#include "../peri_key.h"


void key_init(uint32 gpio_name,uint8 gpio_id,uint8 gpio_func);
void key_intr_handler(struct key_param *key);
uint8 get_gpio_id(uint32 io_name);
uint8 get_gpio_func(uint32 io_name);


#endif
