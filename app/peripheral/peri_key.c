/*******************************************************
 * File name: user_key.c
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

 
#define CONFIG_KEY_NUM            1

LOCAL struct keys_param keys;
LOCAL struct single_key_param *single_key[CONFIG_KEY_NUM];

#define CONFIG_KEY_0_IO_MUX     PERIPHS_IO_MUX_GPIO4_U
#define CONFIG_KEY_0_IO_NUM     4
#define CONFIG_KEY_0_IO_FUNC    FUNC_GPIO4

/******************************************************************************
 * FunctionName : peri_key_short_press
 * Description  : key's short press function, needed to be installed
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
peri_key_short_press(void)
{
    /*struct PLUG_PARAM plug_param;
    plug_param = user_plug_get_status();
    plug_param.status = (~plug_param.status) & 0x01;
    user_plug_set_status(plug_param);*/
	PRINTF("PERI_KEY_SHORT_PRESS occur\n");
}


/******************************************************************************
 * FunctionName : peri_key_long_press
 * Description  : key's long press function, needed to be installed, preserved function.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/

static void ICACHE_FLASH_ATTR
peri_key_long_press(void)
{
	PRINTF("PERI_KEY_LONG_PRESS occur\n"); //pando_gateway_wifi_config();
}

/******************************************************************************
 * FunctionName : peri_key_init.
 * Description  : initialize key device.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_key_init(void)
{

    single_key[0] = key_init_single(CONFIG_KEY_0_IO_NUM, CONFIG_KEY_0_IO_MUX, CONFIG_KEY_0_IO_FUNC,
                                    peri_key_long_press, peri_key_short_press);
    keys.key_num = CONFIG_KEY_NUM;
    keys.single_key = single_key;
    key_init(&keys);
}
