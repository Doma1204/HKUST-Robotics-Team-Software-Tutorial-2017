#include "button.h"

static BUTTON buttons[3];

GPIO_TypeDef* button_ports[3] = {GPIOC, GPIOC, GPIOC};
uc16 button_pins[3] = {GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

/**
  * @brief  Initialization of button GPIO pins
  * @param  None
  * @retval None
  */
void button_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	for (u8 i=0; i<3; i++) {
		buttons[i].port = button_ports[i];
		buttons[i].pin = button_pins[i];
		buttons[i].state = 1;
		buttons[i].has_keydown_listener = 0;
		buttons[i].has_keyup_listener = 0;
	}
}

/**
  * @brief  Read current state of a button
  * @param  id: button to read from
  * @retval 1 if not pressed; 0 if pressed
  */
u8 read_button(BUTTON_ID id) {
	return GPIO_ReadInputDataBit(buttons[id].port, buttons[id].pin);
}

/**
  * @brief  Update state of a button
  * @param  None
  * @retval None
  */
void button_update() {
	for (u8 i=0; i<3; i++) {
		u8 cur_state = read_button((BUTTON_ID)i);
		if (!cur_state && buttons[i].state && buttons[i].has_keydown_listener)
			(*buttons[i].keydown_listener)();
		if (cur_state && !buttons[i].state && buttons[i].has_keyup_listener)
			(*buttons[i].keyup_listener)();
		buttons[i].state = cur_state;
	}
}

/**
  * @brief  Set keydown listener function
  * @param  Pointer to listener function when button pressed
  * @retval None
  */
void set_keydown_listener(u8 id, button_listener *listener) {
	buttons[id].keydown_listener = listener;
	buttons[id].has_keydown_listener = 1;
}

/**
  * @brief  Set keyup listener function
  * @param  Pointer to listener function when button released
  * @retval None
  */
void set_keyup_listener(u8 id, button_listener *listener) {
	buttons[id].keyup_listener = listener;
	buttons[id].has_keyup_listener = 1;
}
