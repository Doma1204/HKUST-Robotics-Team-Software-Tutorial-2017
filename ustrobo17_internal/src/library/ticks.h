#ifndef __TICKS_H
#define __TICKS_H

#include "stm32f10x.h" 

void ticks_init(void);
void ticks_reset(void);
u32 get_ticks(void);
void delay(u32 ms);

#endif	/* __TICKS_H */
