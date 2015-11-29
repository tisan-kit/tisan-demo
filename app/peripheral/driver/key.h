#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"
#include "../peri_key.h"

#define LONG_PRESS_COUNT 3000

void key_5s_cb(struct base_key_param *single_key);
void key_50ms_cb(struct base_key_param *single_key);

#endif
