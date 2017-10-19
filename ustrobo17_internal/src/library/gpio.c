#include "gpio.h"

static GPIO_STRUCT gpio =
	{{GPIO1_PORT,GPIO1_PIN},{GPIO2_PORT,GPIO2_PIN},{GPIO3_PORT,GPIO3_PIN},{GPIO4_PORT,GPIO4_PIN}};
						
void gpio_init(GPIO_ID gpio_id, GPIOMode_TypeDef gpio_mode) {
	if(gpio[gpio_id].port == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	}
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = gpio_mode;
	GPIO_InitStructure.GPIO_Pin = gpio[gpio_id].pin;
	GPIO_Init(gpio[gpio_id].port, &GPIO_InitStructure);
}

u8 gpio_read(GPIO_ID gpio_id) {
	return GPIO_ReadInputDataBit(gpio[gpio_id].port,gpio[gpio_id].pin);
}

void gpio_write(GPIO_ID gpio_id, BitAction BitVal)
{
	GPIO_WriteBit(gpio[gpio_id].port,gpio[gpio_id].pin, BitVal);
}
