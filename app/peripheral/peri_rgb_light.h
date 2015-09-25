/*******************************************************
 * File name: user_tri_color_light.h
 * Author:
 * Versions:1.0
 * Description:This module is driver of the subdevice: tri-colored light.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/
 
#ifndef ___PERI_RGB_LIGHT_H__
#define ___PERI_RGB_LIGHT_H__

#include "c_types.h"
#include "driver/pwm.h"

#pragma pack(1)

struct LIGHT_PARAM 
{
    uint16 pwm_freq;            //100hz typically
    uint8  pwm_duty[PWM_CHANNEL_MAX];         // the sequence is red, green, blue.
    uint8  pad[2];              // the save data must aligned.
};
struct LIGHT_INIT
{
	uint8 io_num;               //pwm io number
	uint8 io_id[PWM_CHANNEL_MAX];	            //list pwm io id
};


void peri_rgb_light_init(struct LIGHT_PARAM light_param,struct LIGHT_INIT light_init);
struct LIGHT_PARAM peri_rgb_light_param_get(void);
void peri_rgb_light_param_set( struct LIGHT_PARAM light_param);
void peri_rgb_light_param_timer_set(void);

#endif
