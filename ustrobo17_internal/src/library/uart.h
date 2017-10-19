#ifndef __UART_H
#define __UART_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include <stdio.h>
#include <stdarg.h>

typedef enum 
{
	UART_NULL = -1,		//disabled
    UART1 = 0,   		//usart1
    UART3 = 1,				//usart3
} UART_TypeDef;

typedef void on_receive_listener(const uint8_t byte);

void uart_init(UART_TypeDef UART, u32 br);
void uart_tx_byte(UART_TypeDef UART, const char data);
void uart_tx(UART_TypeDef UART, const char * tx_buf, ...);
void uart_interrupt(UART_TypeDef UART);
void uart_interrupt_init(UART_TypeDef UART, on_receive_listener *listener);

#endif
