#include "pneumatic.h"

GPIO_TypeDef* pneumatic_ports[4] = {GPIOC, GPIOC, GPIOC, GPIOC};
uc16 pneumatic_pins[4] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3};

void pneumatic_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void pneumatic_control(PNEUMATIC_ID id, u8 state) {
	if (state==1) {
		GPIO_SetBits(pneumatic_ports[id], pneumatic_pins[id]);
	} else {
		GPIO_ResetBits(pneumatic_ports[id], pneumatic_pins[id]);
	}
}
