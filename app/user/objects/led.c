/*******************************************************
 * File name: led.c
 * Author: razr
 * Versions: 1.0
 * Description: the led object
 * History:
 *   1.Date: Sep 15, 2015
 *     Author: razr
 *     Modification:    
 *********************************************************/
#include "../../pando/pando_object.h"
#include "c_types.h"

#define LED_OBJECT_NO 1

struct led {
	uint8 red;
	uint8 green;
	uint8 blue;
	void (*set)(struct led*);
	void (*get)(struct led*);
};

void ICACHE_FLASH_ATTR
led_set(struct led* self)
{
	// user todo. set pwm values
}

void ICACHE_FLASH_ATTR
led_get(struct led* self)
{
	// user todo. read pwm values
}

struct led* ICACHE_FLASH_ATTR
create_led(){
	struct led* led = (struct led*)os_malloc(sizeof(led));
	led->get = led_get;
	led->set = led_set;
	return led;
}

void ICACHE_FLASH_ATTR
delete_led(struct led* led){
	if(led)
	{
		os_free(led);
	}
}

struct TLVs* ICACHE_FLASH_ATTR
led_object_pack()
{
	struct led* led = create_led();
	led->get(led);

	delete_led(led);
}

void ICACHE_FLASH_ATTR
led_object_unpack(struct TLV* params, sint16 count)
{

}


pando_object led_object = {
	LED_OBJECT_NO,
	led_object_pack,
	led_object_unpack,
};




