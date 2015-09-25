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
 
#ifndef ___PERI_MOTOR_H__
#define __PERI_MOTOR_H__

#include "c_types.h"

#pragma pack(1)

struct MOTOR_PARAM
{
    uint16 pwm_freq; //100hz typically
    uint8  pwm_duty[2];//forward and backward
};

void peri_motor_init(void);
struct LIGHT_PARAM peri_rgb_light_param_get(void);
void peri_rgb_light_param_set(struct LIGHT_PARAM light_param);
void peri_rgb_light_param_timer_set(void* arg);

#endif
