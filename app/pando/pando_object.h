/*******************************************************
 * File name: pando_object.h
 * Author: razr
 * Versions: 1.0
 * Description: pando object interfaces
 * History:
 *   1.Date: Sep 11, 2015
 *     Author: razr
 *     Modification:    
 *********************************************************/

#ifndef PANDO_OBJECTS_H_
#define PANDO_OBJECTS_H_

#include "c_types.h"
#include "protocol/sub_device_protocol_tool.h"

typedef struct {
	sint8 no;
	struct TLVs* (*pack)();
	void (*unpack)(struct TLV*, sint16);
}pando_object;

/******************************************************************************
 * FunctionName : register_pando_object.
 * Description  : register a pando object to framework.
 * Parameters   : a pando object.
 * Returns      : none.
*******************************************************************************/
void register_pando_object(pando_object object);

/******************************************************************************
 * FunctionName : find_pando_object.
 * Description  : find a pando object by object no.
 * Parameters   : the object no.
 * Returns      : the pando object of specified no, NULL if not found.
*******************************************************************************/
pando_object* find_pando_object(int8 no);

#endif /* PANDO_OBJECTS_H_ */
