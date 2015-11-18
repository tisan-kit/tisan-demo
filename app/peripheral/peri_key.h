/*******************************************************
 * File name: peri_key.h
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This module is the key function module.

 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/

#ifndef PERI_KEY_H_
#define PERI_KEY_H_

#include "gpio.h"
#include "../../include/os_type.h"
#include "driver/key_base.h"

#define CONFIG_KEY_GPIO_ID     4

void peri_config_key_init(uint8 gpio_id);

#endif /* APP_INCLUDE_USER_USER_KEY_H_ */
