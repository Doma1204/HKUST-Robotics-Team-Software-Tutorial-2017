#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x_gpio.h"

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
}GPIO_STRUCT[];

typedef enum {
	GPIO1,
	GPIO2,
	GPIO3,
	GPIO4
} GPIO_ID;

#define GPIO1_PORT GPIOA
#define GPIO1_PIN  GPIO_Pin_13
#define GPIO2_PORT GPIOA
#define GPIO2_PIN	 GPIO_Pin_14
#define GPIO3_PORT GPIOB
#define GPIO3_PIN	 GPIO_Pin_6
#define GPIO4_PORT GPIOB
#define GPIO4_PIN	 GPIO_Pin_7

void gpio_init(GPIO_ID gpio_id, GPIOMode_TypeDef gpio_mode);
u8 gpio_read(GPIO_ID gpio_id);
void gpio_write(GPIO_ID gpio_id, BitAction BitVal);

#endif /* __GPIO_H */
