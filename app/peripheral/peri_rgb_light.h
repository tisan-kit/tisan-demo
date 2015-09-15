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
 
#ifndef ___USER_TRI_COLOR_LIGHT_H__
#define __USER_TRI_COLOR_LIGHT_H__

#include "c_types.h"

#pragma pack(1)

struct LIGHT_PARAM 
{
    uint16 pwm_freq; //100hz typically
    uint8  pwm_duty[4]; // the sequence is red, green, blue.
    uint8  pad[2]; // the save data must aligned.
};

/******************************************************************************
 * FunctionName : tri-colored light initialize.
 * Description  : initialize tri-colored light .
 * Parameters   : none 
 * Returns      : none
*******************************************************************************/

void peri_rgb_light_init(void);

/******************************************************************************
 * FunctionName : user_tri_color_light_param_get.
 * Description  : get the parameter of the tri-colored light.
 * Parameters   : none 
 * Returns      : the parameter of the tri-colored light.
*******************************************************************************/

struct LIGHT_PARAM peri_rgb_light_param_get(void);

 
/******************************************************************************
 * FunctionName : user_tri_color_light_param_set.
 * Description  : set the parameter of the tri-colored light.
 * Parameters   : light_param-- the parameter set.                  
 * Returns      : the save result
*******************************************************************************/

void peri_rgb_light_param_set(struct LIGHT_PARAM light_param);
void peri_rgb_light_param_timer_set(void* arg);

#endif
