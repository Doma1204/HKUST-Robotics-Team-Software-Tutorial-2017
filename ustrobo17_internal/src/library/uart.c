#include "uart.h"

USART_TypeDef* com_usart[2] = {USART1, USART3};
uc32 com_tx_port_clk[2] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOC};
uc32 com_rx_port_clk[2] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOC};
uc32 com_usart_clk[2] = {RCC_APB2Periph_USART1, RCC_APB1Periph_USART3};
GPIO_TypeDef* com_tx_port[2] = {GPIOA, GPIOC};
GPIO_TypeDef* com_rx_port[2] = {GPIOA, GPIOC};
uc16 com_tx_pin[2] = {GPIO_Pin_9, GPIO_Pin_10};
uc16 com_rx_pin[2] = {GPIO_Pin_10, GPIO_Pin_11};
uc16 com_irq[2] = {USART1_IRQn, USART3_IRQn};

on_receive_listener *uart_rx_listener[2];
u8 has_uart_rx_listener[2] = {0	};

void uart_init(UART_TypeDef UART, u32 br) {
	RCC_APB2PeriphClockCmd(com_rx_port_clk[UART]|com_rx_port_clk[UART]|RCC_APB2Periph_AFIO, ENABLE);
	if (UART == UART1)
		RCC_APB2PeriphClockCmd(com_usart_clk[UART], ENABLE);
	else
		RCC_APB1PeriphClockCmd(com_usart_clk[UART], ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = com_tx_pin[UART];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(com_tx_port[UART], &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = com_rx_pin[UART];
	GPIO_Init(com_rx_port[UART], &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = br;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(com_usart[UART], &USART_InitStructure);
	USART_ITConfig(com_usart[UART], USART_IT_RXNE, ENABLE);
	USART_Cmd(com_usart[UART], ENABLE);
}

void uart_tx_byte(UART_TypeDef UART, char data) {
	while (USART_GetFlagStatus(com_usart[UART], USART_FLAG_TC) == RESET);
	USART_SendData(com_usart[UART], (u16)data);
}

void uart_tx(UART_TypeDef UART, const char *tx_buf, ...) {
	va_list arglist;
	char buf[255], *fp;
	
	va_start(arglist, tx_buf);
	vsprintf(buf, tx_buf, arglist);
	va_end(arglist);
	
	fp = buf;
	while (*fp)
		uart_tx_byte(UART, *fp++);
}

void uart_interrupt(UART_TypeDef UART) {
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef VECT_TAB_RAM
	NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	#else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif
	
	NVIC_InitStructure.NVIC_IRQChannel = com_irq[UART];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(com_usart[UART], USART_IT_RXNE, ENABLE);
}

void uart_interrupt_init(UART_TypeDef UART, on_receive_listener *listener) {
	uart_rx_listener[UART] = listener;
	has_uart_rx_listener[UART] = 1;
	uart_interrupt(UART);
}

void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		if (has_uart_rx_listener[UART1])
			(*uart_rx_listener[UART1])(USART_ReceiveData(USART1));
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		if (has_uart_rx_listener[UART3])
			(*uart_rx_listener[UART3])(USART_ReceiveData(USART3));
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
