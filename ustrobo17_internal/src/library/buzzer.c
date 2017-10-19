#include "buzzer.h"

void buzzer_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
	GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);
}

void buzzer_on(void){
	GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}
void buzzer_off(void){
	GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}

