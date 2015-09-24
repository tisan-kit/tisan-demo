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
#include "../../peripheral/peri_rgb_light.h"

#define LED_OBJECT_NO 1

struct led {
	uint8 red;
	uint8 green;
	uint8 blue;
};

void ICACHE_FLASH_ATTR
led_init()
{
	peri_rgb_light_init();
}

void ICACHE_FLASH_ATTR
led_set(struct led* value)
{
	struct LIGHT_PARAM light_value;
	light_value.pwm_duty[0] = value->red;
	light_value.pwm_duty[1] = value->green;
	light_value.pwm_duty[2] = value->blue;
	peri_rgb_light_param_set(light_value);
}

void ICACHE_FLASH_ATTR
led_get(struct led* value)
{
	struct LIGHT_PARAM light_value;
	light_value = peri_rgb_light_param_get();
	value->red = light_value.pwm_duty[0];
	value->green = light_value.pwm_duty[1];
	value->blue = light_value.pwm_duty[2];
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

void ICACHE_FLASH_ATTR
led_object_pack(PARAMS * params)
{
	if(NULL == params)
	{
		PRINTF("Create first tlv param failed.\n");
		return;
	}

	struct led* led = create_led();
	led_get(led);

	if (add_next_uint8(params, led->red))
	{
		PRINTF("Add next tlv param failed.\n");
		return;
	}

	if (add_next_uint8(params, led->green))
	{
		PRINTF("Add next tlv param failed.\n");
		return;
	}

	if (add_next_uint8(params, led->blue))
	{
		PRINTF("Add next tlv param failed.\n");
		return;
	}

	delete_led(led);
}

void ICACHE_FLASH_ATTR
led_object_unpack(PARAMS* params)
{
	struct led* led = create_led();

	led->red = get_next_uint8(params);
	led->green = get_next_uint8(params);
	led->blue = get_next_uint8(params);

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
