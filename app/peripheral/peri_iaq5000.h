/*******************************************************
 * File name: user_plug.h
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This module is driver of the subdevice: plug.
               in this example:GPIO 15: the plug relay driver;
                               GPIO 12  a blinking led;
                               GPIO 0   the wifi connect state indicate;
                               GPIO 13  a press key.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/

#ifndef __USER_IAQ5000_H__
#define __USER_IAQ5000_H__

struct IAQ5000_PARAM
{
	uint16 iaq;
};

void user_iaq5000_init(void);
uint16 user_iaq5000_read_th(void);
uint8* user_iaq5000_get_poweron_th(void);


#endif

