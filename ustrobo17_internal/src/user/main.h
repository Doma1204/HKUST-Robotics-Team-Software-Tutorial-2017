#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "ticks.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "button.h"
#include "servo.h"
#include "motor.h"
#include "tft.h"
#include "gpio.h"
#include "pneumatic.h"

#define BARCOLOR1     (RGB888TO565(0x002071))
#define BARCOLOR2     (RGB888TO565(0x0D46A0))
#define BGCOLOR       (RGB888TO565(0xF5F5F5))
#define SELECTEDCOLOR (RGB888TO565(0xD0D0D0))
#define TEXTCOLOR1    (RGB888TO565(0x000000))
#define TEXTCOLOR2    (RGB888TO565(0xFFFFFF))

#define button_enter()	button_pressed(BUTTON2)
#define button_next()   button_pressed(BUTTON3)
#define button_back()   button_pressed(BUTTON1)

typedef struct{
  const char* name;
  void (*page_assigned_function)(void);
}PAGE_INFO;

#define NO_OF_PAGES (sizeof(page_info)/sizeof(page_info[0]))

int main(void);
u16 button_pressed(BUTTON_ID button_id);
void page_menu(void);
void page_buzzer_test(void);
void page_led_test(void);
void page_pneumatic_test(void);
void page_gpio_test(void);
void page_uart_test(void);
void uart1_rx_handler(const unsigned char data);
void uart3_rx_handler(const unsigned char data);
void page_motor_test(void);
void page_servo_test(void);
void page_camera_test(void);
void page_ascii_test(void);

#endif	/* __MAIN_H */
