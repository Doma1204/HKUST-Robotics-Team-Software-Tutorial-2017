#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x_gpio.h"

typedef void button_listener(void);

typedef struct{
	GPIO_TypeDef* port;
	u16 pin;
	u8 state;
	u8 has_keydown_listener;
	u8 has_keyup_listener;
	button_listener *keydown_listener;
	button_listener *keyup_listener;
}BUTTON;

typedef enum{
	BUTTON1,
	BUTTON2,
	BUTTON3
}BUTTON_ID;

void button_init(void);
u8 read_button(BUTTON_ID id);
void button_update(void);
void set_keydown_listener(u8 id, button_listener *listener);
void set_keyup_listener(u8 id, button_listener *listener);

#endif
