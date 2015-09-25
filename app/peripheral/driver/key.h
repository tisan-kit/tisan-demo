#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"
#include "../peri_key.h"


void key_init(uint32 gpio_name,uint8 gpio_id,uint8 gpio_func);
void key_intr_handler(struct key_param *key);



#endif
