/*******************************************************
 * File name: pando_config.h
 * Author:
 * Versions:1.0
 * Description:This module is to used to define the parameter and structure used in gateway.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/
 
#ifndef __PANDO_CONFIG_H__
#define __PANDO_CONFIG_H__

#include "user_interface.h"

#ifdef  TEST_ENVEROMENT

#define PANDO_API_URL "http://testapi.pandocloud.com"

#else

#define PANDO_API_URL "http://api.pandocloud.com"

#endif

#define DATANAME_DEVICE_ID "pando_device_id"
#define DATANAME_DEVICE_SECRET "pando_device_secret"
#define DATANAME_DEVICE_KEY "pando_device_key"
#define DATANAME_ACCESS_TOKEN "pando_access_token"
#define DATANAME_ACCESS_ADDR "pando_access_addr"

#define PANDO_DEVICE_MODULE "esp8266"

#define PANDO_SDK_VERSION "1.1.5"


#endif
