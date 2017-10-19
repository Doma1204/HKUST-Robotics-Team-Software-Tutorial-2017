#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x_gpio.h"

#define BUZZER_PORT GPIOC
#define BUZZER_PIN  GPIO_Pin_9


void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);


#endif /* __BUZZER_H */
