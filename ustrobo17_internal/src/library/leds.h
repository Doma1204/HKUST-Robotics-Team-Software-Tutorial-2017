#ifndef __LEDS_H
#define __LEDS_H

#include "stm32f10x.h"

typedef enum{
	LED1,
	LED2,
	LED3
}LED_ID;

void led_init(void);
void led_off(LED_ID id);
void led_on(LED_ID id);

#endif
