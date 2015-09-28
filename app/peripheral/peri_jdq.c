/*******************************************************
 * File name: user_tri_color_light.c
 * Author:
 * Versions:1.0
 * Description:This module is driver of the subdevice: tri-colored light.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/

#include "peri_jdq.h"

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "spi_flash.h"
#include "gpio.h"

uint8 jdq_pin;
/******************************************************************************
 * FunctionName : peri_rgb_light_init
 * Description  : light demo initialize, mainly initialize pwm mode
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_jdq_init(uint8 gpio_id)
{
   // enum JDQ_STATE jdq_state;
    
    PRINTF("I am the jdq\n");
    
    //PRINTF("jdq_state: %d\n", jdq_state);
    PIN_FUNC_SELECT(tisan_get_gpio_name(gpio_id), tisan_get_gpio_general_func(gpio_id));
    PIN_PULLUP_EN(tisan_get_gpio_name(gpio_id));
    jdq_pin=gpio_id;
    
}
/******************************************************************************
 * FunctionName : peri_rgb_light_init
 * Description  : light demo initialize, mainly initialize pwm mode
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_jdq_set_onoff(void)
{
	static uint8 i=0;
	if(i==1)
	{
		 GPIO_OUTPUT_SET(jdq_pin, 1);
	 PRINTF("jdq ON\n");
	 i=0;
	}
	else
	{
		 GPIO_OUTPUT_SET(jdq_pin, 0);
			  PRINTF("jdq OFF\n");
			  i=1;
	}


}


