#ifndef __PNEUMATIC_H
#define __PNEUMATIC_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

typedef enum{
	PNEUMATIC1,
	PNEUMATIC2,
	PNEUMATIC3,
	PNEUMATIC4
}PNEUMATIC_ID;

void pneumatic_init(void);
void pneumatic_control(PNEUMATIC_ID id, u8 state);

#endif
