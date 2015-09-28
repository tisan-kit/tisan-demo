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
 
#ifndef __PERI_JDQ_H__
#define __PERI_JDQ_H__

#include "c_types.h"

#pragma pack(1)

void peri_jdq_init(uint8 gpio_id);
void peri_jdq_set_onoff(void);

#endif
