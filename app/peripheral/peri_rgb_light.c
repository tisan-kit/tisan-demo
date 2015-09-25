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

#include "peri_rgb_light.h"

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "spi_flash.h"



#define PRIV_PARAM_START_SEC		0x7C
#define PRIV_PARAM_SAVE     0

extern struct LIGHT_PARAM light_r_param;
extern struct LIGHT_PARAM light_g_param;
extern struct LIGHT_PARAM light_b_param;

/******************************************************************************
 * FunctionName : peri_rgb_light_param_get.
 * Description  : get the parameter of the RGB light.
 * Parameters   : none 
 * Returns      : the parameter of the RGB light.
*******************************************************************************/

struct LIGHT_PARAM ICACHE_FLASH_ATTR 
peri_rgb_light_param_get(void)
{
    struct LIGHT_PARAM ret;
    
    spi_flash_read((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
    	(uint32 *)&ret, sizeof(struct LIGHT_PARAM));
    return ret;
}

 
/******************************************************************************
 * FunctionName : peri_rgb_light_param_set.
 * Description  : set the parameter of the RGB light.
 * Parameters   : light_param-- RGB light parameter.
 * Returns      : the save result
*******************************************************************************/

void ICACHE_FLASH_ATTR 
peri_rgb_light_param_set( struct LIGHT_PARAM light_param)
{
    pwm_set_freq(light_param.pwm_freq);
    pwm_set_duty(light_param.pwm_duty[0], 0); // red colour.
    pwm_set_duty(light_param.pwm_duty[1], 1); // green colour.
    pwm_set_duty(light_param.pwm_duty[2], 2); // blue colour.
    
    pwm_start();
    
    spi_flash_erase_sector(PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE);
	spi_flash_write((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
	    (uint32 *)&light_param, sizeof(struct LIGHT_PARAM));
}

/******************************************************************************
 * FunctionName : peri_rgb_light_param_timer_set.
 * Description  : set the parameter of the RGB light.
 * Parameters   : light_param-- RGB light parameter.
 * Returns      : the save result
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_rgb_light_param_timer_set(void)
{
	static uint8 j=0;

	if(j==0)
	{
		peri_rgb_light_param_set(light_r_param);
		PRINTF("red\n");
		j++;
	}
	else if(j==1)
	{
		peri_rgb_light_param_set(light_g_param);
		PRINTF("green\n");
		j++;
	}
	else
	{

		peri_rgb_light_param_set(light_b_param);
		PRINTF("blue\n");
		j=0;
	}


	//struct LIGHT_PARAM light_param = *((struct LIGHT_PARAM*)arg);

}
/******************************************************************************
 * FunctionName : peri_rgb_light_init
 * Description  : light demo initialize, mainly initialize pwm mode
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_rgb_light_init(struct LIGHT_PARAM light_param,struct LIGHT_INIT light_init)
{
    //struct LIGHT_PARAM light_param;
    
    PRINTF("I am the tri-colored light\n");
    PRINTF("pwm_freq: %d, pwm_duty_red: %d, pwm_duty_green: %d, pwm_duty_blue: %d\n", light_param.pwm_freq,
        (light_param.pwm_duty)[0], (light_param.pwm_duty)[1], (light_param.pwm_duty)[2]);
   // light_param.pwm_freq = 25000;

   // int i;
   // for(i = 0; i<3; i++ )
   // {
   // 	light_param.pwm_duty[i] = 0;
   // }

    spi_flash_write((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
    	    (uint32 *)&light_param, sizeof(struct LIGHT_PARAM));
            
    pwm_init(light_param.pwm_freq, light_param.pwm_duty,light_init.io_num,light_init.io_id);
    pwm_start();
    
}



