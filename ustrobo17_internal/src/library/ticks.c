#include "ticks.h"

volatile u32 msec = 0;
u8 is_ticks_enabled = 0;

/**
  * @brief  Initialization of ticks timer
  * @param  None
  * @retval None
  */
void ticks_init(void) {

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	SysTick_Config(SystemCoreClock / 1000); 
	
	is_ticks_enabled = 1;
	msec = 0;
}

/**
  * @brief  SysTick interrupt handler
  * @param  None
  * @retval None
  */
void SysTick_Handler(void){
	msec++;
}

/**
  * @brief  Reset the ticks timer
  * @param  None
  * @retval None
  */
void ticks_reset(void) {
	msec = 0;
}

/**
  * @brief  Return no of milliseconds passed
  * @param  None
  * @retval Milliseconds passed
  */
u32 get_ticks(void) {
	return msec;
}

/**
  * @brief  Wait fot the time specified by the user
  * @param  ms
  * @retval None
  */
void delay(u32 ms){
	if(!is_ticks_enabled)ticks_init();
	u32 ticks_img = msec;
	while(ms > 0){
		if(ticks_img != msec){
			ticks_img = msec;
			ms--;
		}
	}
}
