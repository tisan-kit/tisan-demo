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

/******************************************************************************
 * FunctionName : peri_rgb_light_init
 * Description  : light demo initialize, mainly initialize pwm mode
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_jdq_init(void)
{
   // enum JDQ_STATE jdq_state;
    
    PRINTF("I am the jdq\n");
    
    //PRINTF("jdq_state: %d\n", jdq_state);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_PULLUP_EN(PERI_0_OUT_IO_MUX);
   // GPIO_OUTPUT_SET(12, 1);;
    
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
		 GPIO_OUTPUT_SET(12, 1);
	 PRINTF("jdq ON\n");
	 i=0;
	}
	else
	{
		 GPIO_OUTPUT_SET(12, 0);
			  PRINTF("jdq OFF\n");
			  i=1;
	}


}


