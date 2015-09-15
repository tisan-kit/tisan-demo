/*******************************************************
 * File name: pando_gateway.h
 * Author:
 * Versions:1.0
 * Description:This module is the interface of the pando gateway.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/
#include "pando_gateway.h"
#include "user_interface.h"
#include "pando_device_register.h"
#include "pando_device_login.h"
#include "pando_cloud_access.h"
#include "user_interface.h"
#include "pando_system_time.h"
#include "espconn.h"

#define ACEESS_ERROR_MAX_TIMES 3

extern load_data_from_flash();

static gateway_err_callback gateway_err_cb;

static void login_cb(PANDO_LOGIN_RESULT result); //login callback function.

static void access_error_cb(PANDO_ACCESS_ERROR err); //access error callback function.

static void register_cb(PANDO_REGISTER_RESULT result); // register callback function.

/******************************************************************************
 * FunctionName : login_cb
 * Description  : the callback function after device login pando cloud.
 * Parameters   : the login result.
 * Returns      : none
*******************************************************************************/
static void ICACHE_FLASH_ATTR
login_cb(PANDO_LOGIN_RESULT result)
{
    PRINTF("PANDO login result: %d\n", result);
    
    if(result == LOGIN_OK)
    {
        pando_cloud_access(access_error_cb);

    }
    else if(result == ERR_NOT_REGISTERED)
    {
        pando_device_register(register_cb);
    }
    else
    {
    	if(gateway_err_cb != NULL)
    	{
    		gateway_err_cb(ERR_LOGIN);
    	}
    }
}

/******************************************************************************
 * FunctionName : register_cb
 * Description  : the callback function after device register pando cloud.
 * Parameters   : the register result.
 * Returns      : none
*******************************************************************************/
static void ICACHE_FLASH_ATTR
register_cb(PANDO_REGISTER_RESULT result)
{
    PRINTF("PANDO register result: %d\n", result);

    if(result == REGISTER_OK)
    {
    	pando_device_login(login_cb);
    }
    else
    {
    	if(gateway_err_cb != NULL)
    	{
    		gateway_err_cb(ERR_REGISTER);
    	}
    }
}

/******************************************************************************
 * FunctionName : access_error_cb
 * Description  : the callback function when device access pando cloud wrong.
 * Parameters   : the access error cause.
 * Returns      : none
*******************************************************************************/
static void ICACHE_FLASH_ATTR
access_error_cb(PANDO_ACCESS_ERROR err)
{
    PRINTF("PANDO access error: %d\n", err);

    static uint8 access_times = 0;
    if(access_times >= ACEESS_ERROR_MAX_TIMES)
    {
        if(gateway_err_cb != NULL)
        {
        	gateway_err_cb(ERR_ACCESS);
        }
    }
    else
    {
    	access_times++;
        pando_cloud_access(access_error_cb);
    }
}

/******************************************************************************
 * FunctionName : pando_gateway_init
 * Description  : initialize the gateway.
 * Parameters   : gateway_err_callback err_cb : the callback function when gateway \
 * 				  connect pando cloud error.
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
pando_gateway_init(gateway_err_callback err_cb)
{
    PRINTF("PANDO gateway initial....\n");

    if(err_cb != NULL)
    {
    	gateway_err_cb = err_cb;
    }

    espconn_secure_set_size(ESPCONN_CLIENT,5120);

	pando_system_time_init();

    load_data_from_flash();

    pando_device_login(login_cb);
}

