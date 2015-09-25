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

#include "peri_motor.h"

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "spi_flash.h"


/******************************************************************************
 * FunctionName : peri_motor_param_set.
 * Description  : set the parameter of the peripheral motor.
 * Parameters   : MOTOR_PARAM.
 * Returns      : the save result
*******************************************************************************/

void ICACHE_FLASH_ATTR 
peri_motor_param_set(struct MOTOR_PARAM motor_param)
{
    pwm_set_freq(motor_param.pwm_freq);
    pwm_set_duty(motor_param.pwm_duty[0], 0);
    pwm_set_duty(motor_param.pwm_duty[1], 1);
    
    pwm_start();
}

/******************************************************************************
 * FunctionName : peri_motor_param_set.
 * Description  : set the parameter of the peripheral motor.
 * Parameters   : MOTOR_PARAM.
 * Returns      : the save result
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_motor_param_timer_set(void* arg)
{
	struct MOTOR_PARAM motor_param = *((struct MOTOR_PARAM*)arg);
	peri_motor_param_set(motor_param);
}
/******************************************************************************
 * FunctionName : peri_motor_init
 * Description  : motor initialize, mainly initialize pwm mode
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_motor_init(void)
{
    struct MOTOR_PARAM motor_param;
    
    PRINTF("I am an electronic motor\n");
    
    motor_param.pwm_freq = 25000;

    motor_param.pwm_duty[0] = 0;
    motor_param.pwm_duty[1] = 0;

            
    PRINTF("pwm_freq: %d, pwm_forward_duty: %d, pwm_backward_duty: %d", motor_param.pwm_freq,
    		(motor_param.pwm_duty)[0],(motor_param.pwm_duty)[1]);
            
    //pwm_init(motor_param.pwm_freq, motor_param.pwm_duty);
    
    pwm_start();
    
}



