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
 
#ifndef __PANDO_GATEWAY_H__
#define __PANDO_GATEWAY_H__

typedef enum {
    ERR_LOGIN = 0,
    ERR_REGISTER,
    ERR_ACCESS
}GATEWAY_ERROR;

/*
 * "register_callback" is a callback function invoked when device register is finished.
 * "result" returns the register return code.
 */
typedef void (*gateway_err_callback)(GATEWAY_ERROR err);

/******************************************************************************
 * FunctionName : pando_gateway_init
 * Description  : initialize the gateway.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void pando_gateway_init(gateway_err_callback  err_cb);

#endif
