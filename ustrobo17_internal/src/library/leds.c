#include "leds.h"

GPIO_TypeDef* led_ports[3] = {GPIOA, GPIOC, GPIOD};
uc16 led_pins[3] = {GPIO_Pin_6, GPIO_Pin_12, GPIO_Pin_2};

/**
  * @brief  Initialization of LED GPIO pins
  * @param  None
  * @retval None
  */
void led_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

/**
  * @brief  Turn off a LED
  * @param  id: LED to turn off
  * @retval None
  */
void led_off(LED_ID id) {
	GPIO_ResetBits(led_ports[id], led_pins[id]);
}

/**
  * @brief  Turn on a LED
  * @param  id: LED to turn on
  * @retval None
  */
void led_on(LED_ID id) {
	GPIO_SetBits(led_ports[id], led_pins[id]);
}
