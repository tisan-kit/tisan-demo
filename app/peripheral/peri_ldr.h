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
 
#ifndef __PERI_LSR_H__
#define __PERI_LSR_H__

#include "c_types.h"

#pragma pack(1)

uint16 peri_lsr_read(void);
void peri_lsr_timer_read(void);

#endif
