/*******************************************************
 * File name: pando_object.c
 * Author: razr
 * Versions: 1.0
 * Description: 
 * History:
 *   1.Date: Sep 11, 2015
 *     Author: razr
 *     Modification:    
 *********************************************************/

#include "pando_object.h"

#define MAX_OBJECTS 16

static pando_object s_pando_object_list[MAX_OBJECTS] = {};
static s_pando_object_list_idx = 0;

void ICACHE_FLASH_ATTR
register_pando_object(pando_object object)
{
	if(s_pando_object_list_idx > MAX_OBJECTS - 1)
	{
		return;
	}

	s_pando_object_list[s_pando_object_list_idx++];
}

pando_object* ICACHE_FLASH_ATTR
find_pando_object(int8 no)
{
	int i;
	for(i = 0; i < s_pando_object_list_idx; i++)
	{
		if( s_pando_object_list[i].no == no)
		{
			return &s_pando_object_list[i];
		}
		return NULL;
	}
}
