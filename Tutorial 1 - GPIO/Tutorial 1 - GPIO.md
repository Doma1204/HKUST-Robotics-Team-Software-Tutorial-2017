# Tutorial 1 - GPIO

Author: Emmett Yim<br>
Contact: yhyim@ust.hk

---

## Basic
### -Introduction-
**General-purpose input/output** (**GPIO**) is a generic pin on an integrated circuit or computer board whose behavior, including whether it is an *input* or *output* pin, is controllable by the user at run time.
---
### -Fundamental Program Structure-
1. Double click **ustrobo17_internal>ustrobo17_internal.uvprojx**
2. Open **user>main.c** on the left
3. During this tutorial, only change the program in main.c
```C
//including header files
#include "main.h"

int main()
{
   //initialization, order matters
   ticks_init();
   led_init();
   
   //variables
   uint32_t ticks = 0;
   uint8_t ledOn = 1;//used as a flag to store state of LED1
   
   //an infinite loop
   while(1)
   {
      //regulates time interval to every ms
      if(get_ticks() != ticks)
      {
         //get_ticks() returns the time past after ticks_init() is called, in ms
	 //updating the value stored in ticks
	 ticks = get_ticks();
	 
	 //toggle led1 every 100ms
	 //ticks % PERIOD == REMAINDER
	 if(ticks % 100 == 0)
	 {
	    if(ledOn)
	       led_on(LED1);
	    else
	       led_off(LED1);
	    ledOn = !ledOn;
	 }
	 
	 //some other tasks
	 if(ticks % 50 == 5)
	 {
	    //e.g. button checking
	    //input reading usually need to be updated more frequently
	 }
	 
	 if(ticks % 100 == 11)
	 {
	    //e.g. buzzer control
	    //output can be updated less frequently to save computation power and distinguishable for human
	 }
	 
	 if(ticks % 25 == 19)
	 {
	    //...
	 }
      }
   }
}

```
#### -Remark:
Try to replace ```int, long, long long, float, double``` for embedded system programming with the followings:

| Full name | Short form | Meaning |
| :---: | :---: | :---: |
| uint8_t | u8 | unsigned 8 bits |
| uint16_t | u16 | unsigned 16 bits |
| uint32_t | u32 | unsigned 32 bits |
| int8_t | s8 | signed 8 bits |
| int16_t | s16 | signed 16 bits |
| int32_t | s32 | signed 32 bits |
| *uint64_t* | *u64* | unsigned 64 bits |
| *int64_t* | *s64* | signed 64 bits |
---
### -GPIO Configuration and i/o-
#### -Initialization
 In **library>gpio.h**, the following can be found:
* Function prototype
```C
void gpio_init(GPIO_ID gpio_id, GPIOMode_TypeDef gpio_mode);
```
```gpio_id``` indicates which GPIO to be initialized; ```gpio_mode``` indicates it is input / output

* Definition of GPIO_ID
```C
typedef enum {
   GPIO1,
   GPIO2,
   GPIO3,
   GPIO4
} GPIO_ID;
```
 In **stm32f10x_std>stm32f10x_gpio.h**, the definition of **GPIOMode_TypeDef** can be found:
```C
typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;
```
 For input, use ``` GPIO_Mode_IPU ``` / ```GPIO_Mode_IPD``` depending on the hareware connection.<br>
 For output, use ``` GPIO_Mode_Out_PP ```.
 
* Example:
```C
//***GPIO init***
//input
gpio_init(GPIO1,GPIO_Mode_IPU);
gpio_init(GPIO2,GPIO_Mode_IPU);
gpio_init(GPIO3,GPIO_Mode_IPD);
gpio_init(GPIO4,GPIO_Mode_IPD);
//output
gpio_init(GPIO1,GPIO_Mode_Out_PP);
gpio_init(GPIO2,GPIO_Mode_Out_PP);
gpio_init(GPIO3,GPIO_Mode_Out_PP);
gpio_init(GPIO4,GPIO_Mode_Out_PP);
```

#### -Read GPIO Input
 In **library>gpio.h**, the following can be found:
* Function prototype
```C
u8 gpio_read(GPIO_ID gpio_id);
```
```gpio_id``` indicates which GPIO to be read

* Example:
```C
//capturing the input(1 for high, 0 for low) from GPIO1 if IPD is set
u8 input = gpio_read(GPIO1);
```

#### -Write GPIO Output
 In **library>gpio.h**, the following can be found:
* Function prototype:
```C
void gpio_write(GPIO_ID gpio_id, BitAction BitVal);
```
```gpio_id``` indicates which GPIO to be written; ```BitVal``` indicates it is written as digital high / low

 In **stm32f10x_std>stm32f10x_gpio.h**, the following can be found:
* Definition of BitAction:
```C
typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;
```
* ```Bit_RESET``` means low
* ```Bit_SET``` means high

* Example:
```C
//set GPIO1 to high
gpio_write(GPIO1,Bit_SET);
//set GPIO1 to low
gpio_write(GPIO1,Bit_RESET);
```
---
### -LED control-
#### -Initialization
 In **library>leds.h**, the following can be found:
* Function prototype
```C
void led_init(void);
```

#### -Led on
 In **library>leds.h**, the following can be found:
* Function prototype
```C
void led_on(LED_ID id);
```
```id``` indicates which led to be switched on

* Definition of LED_ID
```C
typedef enum{
   LED1,
   LED2,
   LED3
}LED_ID;
```
LED1 labelled as A1; LED2 labelled as B1; LED3 labelled as C;
* Example:
```C
//switch on led 1
led_on(LED1);
```

#### -Led off
 In **library>leds.h**, the following can be found:
* Function prototype
```C
void led_off(LED_ID id);
```
```id``` indicates which led to be switched off

* Example:
```C
//switch off led 1
led_off(LED1);
```

#### ***Classwork***
```
Construct a program that:
 - leds will light up in sequence<br>
 [LED1 on]>[LED2 on]>[LED3 on]>[LED1 on]>[LED2 on]>...
Reminder:
 - call led_init()
 - the order of init function matters 
```
#### Remark : update the leds.c file if needed
```C
//inside leds.c > led_init()
//   change
RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOB   |RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
//   to
RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD   |RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
```

---
### -Pneumatic control-
#### -Initialization
 In **library>pneumatics.h**, the following can be found:
* Fucntion prototype
```C
void pneumatic_init(void);
```

#### -Pneumatic control
 In **library>pneumatics.h**, the following can be found:
* Fucntion prototype
```C
void pneumatic_control(PNEUMATIC_ID id, u8 state);
```
```id``` indicates which pneumatic port to be controlled; ```state``` indicates it is digital high / low

#### -Remark:
```state``` can either be ```1``` or ```0```

* Definition of PNEUMATIC_ID
```C
typedef enum{
   PNEUMATIC1,
   PNEUMATIC2,
   PNEUMATIC3,
   PNEUMATIC4
}PNEUMATIC_ID;
```
* Example:
```C
//set bit
pneumatic_control(PNEUMATIC1,1);
//reset bit
pneumatic_control(PNEUMATIC1,0);
```
---
### -Button-
#### -Initialization
 In **library>button.h**, the following can be found:
* Fucntion prototype
```C
void button_init(void);
```

#### -Button input
 In **library>button.h**, the following can be found:
* Fucntion prototype
```C
u8 read_button(BUTTON_ID id);
```
```id``` indicates which button's state to be read

* Definition of BUTTON_ID
```C
typedef enum{
   BUTTON1,
   BUTTON2,
   BUTTON3
}BUTTON_ID;
```
* Example:
```C
u8 pressed = read_button(BUTTON1);
```
#### -Remark:
```input``` will be ```0``` when the button is pressed, and vise versa,<br>
because there is a **pull up resistor** inside MCU and ```GPIO_Mode``` is set to ```IPU```

#### ***Classwork***
```
1. Construct a program that:
 - LEDx will light up only when BUTTONx is pressed

2. Construct a program that:
 - LEDx is toggled when BUTTONx is pressed

3. Construct a program that:
 - LEDx will light up only when BUTTONx is pressed
 - If multiple buttons are pressed, only LEDx will light up where BUTTONx is the first button pressed.

4. Construct a program that:
 - LEDx will light up only when BUTTONx is pressed
 - If multiple buttons are pressed, only LEDx will light up where BUTTONx is the first button pressed.
 - LEDx will flash once per 300*x ms when BUTTONi is pressed.

5.Construct a program that:
 - leds will light up in sequence while any button is held on<br>
   [LED1 on]->[LED2 on]->[LED3 on]->[LED1 on]->[LED2 on]->...
 - upon releasing buttons, the previous procedue stops and only the selected led remains to be on
 
Reminder:
 - call button_init()
 - include the "button.h" header file in "main.h" if haven't
```

---
### -Software debouncing-
#### -Definition
**Bouncing** is the tendency of any two metal contacts in an electronic device to generate multiple signals as the contacts close or open.<br>
**Debouncing** is any kind of hardware device or software that ensures that only a **single signal** will be acted upon for a single opening or closing of a contact.

#### -Implementation
```C
if(ticks % 50 == 1)
{
   static u8 debounce;
   //reset debounce if button is not pressed
   if(read_button(BUTTON1) && debounce)
      debounce = 0;
   
   //set debounce if button is initially pressed
   if(!read_button(BUTTON1) && !debounce)
      debounce = 1;
   
   //handle button triggered event after debouncing
   else if(!read_button(BUTTON1) && debounce)
   {
      //button triggered program here...
   }
}
```
---
### -Buzzer-
#### -Initialization
```C
void buzzer_init(void);
```

#### -Buzzer on
```C
void buzzer_on(void);
```

#### -Buzzer off
```C
void buzzer_off(void);
```

#### -Remark:
* It can be used to show state / debug, similar to leds
* Please do not distroy my ears
* Play music?

#### *Classwork*
```
1. Construct a program that:
 - Include "buzzer.h" in "main.h"
 - BUZZER will beep once per 1000*x ms when BUTTONx is pressed.
 - If multiple buttons are pressed, the BUZZER will only beep at the corresponding rate of the first button pressed.
```

---

## Extra
---
### -GPIO Setup-

Taking the gpio_init for GPIO1 as an example:
```C
//GPIO setup for GPIO1(PA13)
GPIO_InitTypeDef GPIO_InitStructure;
//enable the peripheral clock for GPIOA
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//set the pin as GPIO_Pin_13
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
//set the gpio mode to input pull up
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//set the speed as 50MHz
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//initialize it to gpio port GPIOA
GPIO_Init(GPIOA, &GPIO_InitStructure);
```
There are four parts:
1. GPIO port - GPIOx
2. GPIO pin - GPIO_Pin_x
3. Peripheral clock - RCC_APB2Periph_GPIOx
4. GPIO Mode
5. GPIO speed - GPIO_Speed_50MHz(set as this normally)

#### -GPIO port & GPIO pin
There are **4 peripherals** (GPIOA,GPIOB,GPIOC,GPIOD), each would have 16 pins(from 0 to 15) maximum.<br>
GPIO1 in this case is assigned to ```PA13```,where A means GPIOA and 13 means Pin13<br>
Therefore, we set ```GPIO_Pin = GPIO_Pin_13``` and initialize it with ```GPIO_Init(GPIOA, &GPIO_InitStructure);```

#### -Peripheral clock
Each GPIOx has its own peripheral clock for its own timing purpose.<br>
```APB2``` means it is a high speed peripheral clock(72MHz, as fast as the MCU frequency).

#### -GPIO_Mode
Among
```C
typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;
```
only ```GPIO_Mode_IPD```, ```GPIO_Mode_IPU```, ```GPIO_Mode_Out_OD``` and ```GPIO_Mode_Out_PP``` will be covered.

#### -GPIO_Mode_IPD
**Input Pull Down** (**IPD**) has to be set when the GPIO is connected to a pull down resistor.<br>

 Hardware connection:<br>
![when the GPIO is connected to a pull down resistor](https://i.imgur.com/sd7FdFC.jpg)

#### -GPIO_Mode_IPU
**Input Pull Up** (**IPU**) has to be set when the GPIO is connected to a pull up resistor.<br>

 Hardware connection:<br>
![when the GPIO is connected to a pull up resistor](https://i.imgur.com/iS3Od90.jpg)

#### -GPIO_Mode_Out_OD
**Output Open Drain** (**Out_OD**) can be used for the following function:
1. communication pin setting (e.g. i2c).
2. power external circuit which would consume higher voltage(>3.3V) with suitable hardware circuit.
3. many other usage that you may do research yourself.

#### -GPIO_Mode_Out_PP
**Output Push Pull** (**Out_PP**) outputs ```VCC``` when GPIO is set to ```high```; ```GND``` when GPIO is set to ```low```.<br>

Hardware connection:<br>
![outputs ```VCC``` when GPIO is set to ```high```; ```GND``` when GPIO is set to ```low```](https://i.imgur.com/UerwY9k.png)

#### -GPIO speed
GPIO speed is the maximum ```SET``` / ```RESET``` access you can make every second.<br>
Normally, it is set to its maximum supported speed by hardware : ``` GPIO_Speed_50MHz```<br>
```C
typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
```

---
### -EXTI & NVIC-

*the following code is taking GPIO1 (PA13) as an example*

#### -Definition of EXTI
**EXTI** means **external interrupt**.<br>
Interrupt is a prioritized event that would be handled first at any time, while temporarily pausing the current event until the interrupt is finished.<br>
External interrupt is an interrupt triggered by an external device through GPIO, for example the input from a limit switch.<br>
**Advantage** : does not need to perform GPIO input checking with gpio_read frequently<br>
**Disavantage** : interrupt will happened for multiple times if there is input bouncing as EXTI has no hardware debouncing while software debouncing is also not applicable.

#### -EXTI setup example
```C
EXTI_InitTypeDef EXTI_InitStructure;
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);
EXTI_InitStructure.EXTI_Line = EXTI_Line13;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//interrupt triggered by rising edge
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
//interrupt triggered by falling edge
//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
EXTI_GenerateSWInterrupt(EXTI_Line13);
```

#### -Definition of NVIC
**NVIC** means **Nested Vectored Interrupt Controller**.<br>
It controlls the handling of different interrupts(*EXTI, ADC and TIM*) and their priorities.

#### -NVIC setup example
```C
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

#### -IRQ Handler example
```C
//checking the flag for external interupt for line 13
if ( EXTI_GetITStatus(EXTI_Line13) != RESET )
{
   //your code here
   //...
   
   //clear the flag
   EXTI_ClearITPendingBit(EXTI_Line13);
}
```

#### -Sample setup of GPIO1 as external interrupt source
```C
//GPIO setup
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
//EXTI setup
EXTI_InitTypeDef EXTI_InitStructure;
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);
EXTI_InitStructure.EXTI_Line = EXTI_Line13;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//interrupt triggered by rising edge
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
//interrupt triggered by falling edge
//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
EXTI_GenerateSWInterrupt(EXTI_Line13);
//NVIC setup
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

#### -Pin configuration reference for EXTI

| Pin | GPIO_Pin | GPIO_PinSource | EXTI_Line | NVIC_IRQChannel | IRQHandler |
| :---: | :---: | :---: | :---: | :---: | :---: |
| Pin 0 | GPIO_Pin_0 | GPIO_PinSource0 | EXTI_Line0 | EXTI0_IRQn | EXTI0_IRQHandler |
| Pin 1 | GPIO_Pin_1 | GPIO_PinSource1 | EXTI_Line1 | EXTI1_IRQn | EXTI1_IRQHandler |
| Pin 2 | GPIO_Pin_2 | GPIO_PinSource2 | EXTI_Line2 | EXTI2_IRQn | EXTI2_IRQHandler |
| Pin 3 | GPIO_Pin_3 | GPIO_PinSource3 | EXTI_Line3 | EXTI3_IRQn | EXTI3_IRQHandler |
| Pin 4 | GPIO_Pin_4 | GPIO_PinSource4 | EXTI_Line4 | EXTI4_IRQn | EXTI4_IRQHandler |
| Pin 5 - 9 | GPIO_Pin_5 to 9 | GPIO_PinSource5 to 9 | EXTI_Line9_5 | EXTI9_5_IRQn | EXTI9_5_IRQHandler |
| Pin 10 - 15 | GPIO_Pin_10 to 15 | GPIO_PinSource10 to 15 | EXTI_Line15_10 | EXTI15_10_IRQn | EXTI15_10_IRQHandler |

---

## Homework
```
release during your tutorial section via email
```
