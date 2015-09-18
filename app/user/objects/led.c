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
#include "user_interface.h"

#define LED_OBJECT_NO 1

struct led {
	uint8 red;
	uint8 green;
	uint8 blue;
};

void ICACHE_FLASH_ATTR
led_init()
{
	// TODO. init led here.
}

void ICACHE_FLASH_ATTR
led_set(struct led* self)
{
	// TODO. set pwm values
}

void ICACHE_FLASH_ATTR
led_get(struct led* self)
{
	// TODO. read pwm values
}


struct led* ICACHE_FLASH_ATTR
create_led()
{
	struct led* led = (struct led*)os_malloc(sizeof(led));
	return led;
}

void ICACHE_FLASH_ATTR
delete_led(struct led* led)
{
	if(led)
	{
		os_free(led);
	}
}

struct TLVs* ICACHE_FLASH_ATTR
led_object_pack()
{
	struct led* led = create_led();
	led_get(led);
	struct TLVs *params_block = create_params_block(TLV_TYPE_UINT8, sizeof(uint8), &led->red);

	if (params_block == NULL)
	{
		PRINTF("Create first tlv param failed.\n");
		return NULL;
	}

	if (add_next_param(params_block, TLV_TYPE_UINT8, sizeof(uint8), &led->green))
	{
		delete_params_block(params_block);
		PRINTF("Add next tlv param failed.\n");
		return NULL;
	}

	if (add_next_param(params_block, TLV_TYPE_UINT8, sizeof(uint8), &led->blue))
	{
		delete_params_block(params_block);
		PRINTF("Add next tlv param failed.\n");
		return NULL;
	}

	delete_led(led);

	return params_block;
}

void ICACHE_FLASH_ATTR
led_object_unpack(struct TLV* params, sint16 count)
{
	int i = count;
	struct led* led = create_led();

	uint16_t tlv_type, tlv_length;
	uint8_t value;


	get_tlv_param(params, &tlv_type, &tlv_length, &value);
	PRINTF("params: type: %02x, length: %02x, \n", tlv_type, tlv_length);
	show_package(&value, tlv_length);
	led->red = value;
	get_tlv_param(params, &tlv_type, &tlv_length, &value);
	PRINTF("params: type: %02x, length: %02x, \n", tlv_type, tlv_length);
	show_package(&value, tlv_length);
	led->green = value;
	get_tlv_param(params, &tlv_type, &tlv_length, &value);
	led->blue = value;
	PRINTF("params: type: %02x, length: %02x, \n", tlv_type, tlv_length);
	show_package(&value, tlv_length);

	led_set(led);

	delete_led(led);

}

void ICACHE_FLASH_ATTR
led_object_init()
{
	// init here...
	led_init();

	// register the object
	pando_object led_object = {
		LED_OBJECT_NO,
		led_object_pack,
		led_object_unpack,
	};
	register_pando_object(led_object);
}




