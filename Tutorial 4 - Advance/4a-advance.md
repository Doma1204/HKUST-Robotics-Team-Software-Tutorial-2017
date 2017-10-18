# Tutorial 4a - Advance Progarm Structure and Library (tft & camera)

Author: Emmett Yim<br>Contact: [yhyim@ust.hk](mailto:yhyim@ust.hk) / 66816823(Whatsapp)

## -Advance Program Structure-

### -File management on Keil-
As sometimes we would collaborate with other programmer or we would like to keep the ```main.c``` function neat and tidy, we would create libraries to achieve these goals. The following will be a basic guide of library file management.<br>

#### -Importing libraries-
1. Place the library files (.c and .h) in the correct location : ```ustrobo17_internal > src > library```
2. Go to Keil
3. In the ```Project``` panel on the left
4. Double click the **word** ```library```
5. Select the correct path to the files : ```ustrobo17_internal > src > library```
6. Select the ```.c``` file (you may also add the ```.h``` file for convenience)
7. Click ```Add```
8. Include ```#include "LIBRARY_NAME.h"``` in the required ```.h``` files
9. Press **F7** to compile

#### -Creating new libraries-
1. Click ```File > New...```, or press **ctrl** + **N**
2. Type your codes in the new files
3. Click ```File > Save```, or press **ctrl** + **S**
4. Select the correct location : ```ustrobo17_internal > src > library```
5. Click ```Save```
6. Import the files to the project with the method above

### -Library Structure-
This part will introduce the basic mindset you need to have for building a library. Followed by some useful coding convention for our library.

#### -Header file-
- Make use of ```#ifndef```(if not define), ```#define``` and ```endif``` to avoid multiple declaration<br>
```C
#ifndef __NEW_LIBRARY_H
#define __NEW_LIBRARY_H
/* ... */
#endif //__NEW_LIBRARY_H
```
- Consider what existing header file you need to include<br>e.g.```#include "stm32f10x.h"``` for ```uintX_t / intX_t``` datatype
- You may use ```typedef enum``` to increase the readability<br>
```C
typedef enum
{
  CaseA = 0,
  CaseB,
  CaseC
}Case;
```
- You may use ```type struct``` to organize variables<br>
```C
typedef struct
{
  BUTTON_ID id;
  uint8_t state;
  uint32_t ticks;
}ButtonStruct;
```
- You may use ```#define``` to create constant / macro fucntion

#### -C file-
- Include its own header file(s)
- You may use ```#define``` to create constant
- You may use ```#define```, ```#ifdef```, ```#ifndef``` and ```#endif``` to do branching<br>
In fact, it is handled by the compiler, thus the unused branches will not be compiled. Hence reducing file size.
- Usually, there will be an initialization function<br>
  - Variables initialization
  - GPIO / TIM / EXTI / NVIC or other necessary configuration
- Consider what **input/output** or **read/write** functions are required
- Consider is there anything that has to be check frequently / update procedurally<br>
creating a ```LIBRARY_NAME_update``` function might help you

### -Finite State Machine-
A finite state machine(FSM) is a model of a task/procedure with a finite number of states/steps, which would act accordingly(go to another state) by considering its previous state and current state.<br>
Let rollback to **Tutorial 1 - GPIO** homework.<br>
```
- when BUTTON1 is just pressed, switch on the previous led and switch off the current led
 [LED1 on]->[LED3 on] or [LED3 on]->[LED2 on] or [LED2 on]->[LED1 on]
- after holding BUTTON1 for 500ms, the leds will light up in a sequence of
  ...->[LED1 on]->[LED3 on]->[LED2 on]->[LED1 on]->[LED3 on]->...
- when BUTTON1 is released, the previous procedue stops and only the selected led remains to be on
```

#### Implementation with Switch-Case
```C
switch(buttonState)
{
  case notBeingPressed:
    if(!read_button(BUTTON1))
    {
      buttonState = justBeingPressed;
      buttonTicks = ticks;
      /* led control */
    }
    break;
  
  case justBeingPressed:
    if(!read_button(BUTTON1) && get_ticks() - buttonTicks >= 500)
    {
      buttonState = beingHeld;
      buttonTicks = ticks;
      /* led control */
    }
    else if(read_button(BUTTON1))
    {
      buttonState = notBeingPressed;
      buttonTicks = 0;
    }
    break;
  case beingHeld:
    if(!read_button(BUTTON1) && get_ticks() - buttonTicks >= 100)
    {
      buttonTicks = ticks;
      /* led control */
    }
    else if(read_button(BUTTON1))
    {
      buttonState = notBeingPressed;
      buttonTicks = 0;
    }
    break;
}
```
- **pro** : new cases can be add at any time without affecting the whole structure
- **con** : only **ONE** state can be received for determining which case

#### Implementation with Nested If-Else
```C
if(buttonState == notBeingPressed && !read_button(BUTTON1))
{
  buttonState = justBeingPressed;
  buttonTicks = ticks;
  /* led control */
}
else if(buttonState = justBeingPressed && !read_button(BUTTON1) && get_ticks() - buttonTicks >= 500)
{
  buttonState = beingHeld;
  buttonTicks = ticks;
  /* led control */
}
else if(buttonState = beingHeld && !read_button(BUTTON1) && get_ticks() - buttonTicks >= 100)
{
  buttonTicks = ticks;
  /* led control */
}
else if(read_button(BUTTON1))
{
  buttonState = notBeingPressed;
  buttonTicks = 0;
}
```
- **pro** : multiple states can be considered at the same time
- **con** : the if-condition might be lengthy and many changes might have to be made when adding / removing states or conditions

## -Library (tft & camera)-

### -TFT-
As the tft library is a bit complex, only some useful functions will be introduced

#### tft_init
```C
void tft_init(u8 orientation, u16 in_bg_color, u16 in_text_color, u16 in_text_color_sp);
```
- **orientation** : ranged from 0 to 3, representing 4 orientation respectively<br>
Entering 0 would takes the position of the tft pins as top, then turn in the clockwise direction for 1 to 3.<br>
***the following will assume orientation 0 / 2 are set***
- **in_bg_color**, **in_text_color** and **in_text_color_sp** : colour for background, text, and special text respectively<br>
Where the colour is in RGB565 format(0x0000 to 0xFFFF). You can enter it directly or use the following default colour : <br>
```C
#define WHITE       (RGB888TO565(0xFFFFFF))
#define BLACK       (RGB888TO565(0x000000))
#define DARK_GREY   (RGB888TO565(0x555555))
#define GREY        (RGB888TO565(0xAAAAAA))
#define RED         (RGB888TO565(0xFF0000))
#define ORANGE      (RGB888TO565(0xFF7700))
#define YELLOW      (RGB888TO565(0xFFFF00))
#define GREEN       (RGB888TO565(0x00FF00))
#define	DARK_GREEN  (RGB888TO565(0x00CC00))
#define BLUE        (RGB888TO565(0x0000FF))
#define	BLUE2       (RGB888TO565(0x202060))
#define	SKY_BLUE    (RGB888TO565(0x11CFFF))
#define CYAN        (RGB888TO565(0x8888FF))
#define PURPLE      (RGB888TO565(0x660066))
```

#### tft_clear
```C
void tft_clear(void); 		//clear everything on tft
void tft_clear_line(u8 line);	//clear one row
```
Function for clearing the tft screen<br>
If ```tft_clear``` is used, it has to be called **before** any tft print function<br>
- **line** : n-th line to be cleared, ranged from 0 to 9

#### tft_prints
```C
void tft_prints(u8 x, u8 y, const char * pstr, ...);
```
- **x** : n-th charactor's position of a line, ranged from 0 to 15 (i.e. maximum 16 charactors per line)
- **y** : n-th line, ranged from 0 to 9 (i.e. 10 lines in total)
- **const char * pstr** : string output, just like how you use ```uart_tx()```

#### tft_put_pixel
```C
void tft_put_pixel(u8 x, u8 y, u16 color);
```
- **x** : n-th horizontal pixel, ranged from 0 to 127 (i.e. 128 pixels per row)
- **y** : n-th vertical pixel, ranged from 0 to 159 (i.e. 160 pixels per column)
- **color** : colour of the pixel, also in RGB565 format


***Remark1 : useful in printing camera output<br>Remark2 : if I mixed up row and column, please bear with me ***=]

#### tft_update
```C
void tft_update(void);
```
Function for outputing the data stored inside the buffer to tft<br>
It has to be called **after** all tft print function

### -Camera - ov7725-
ov7725 is the camera module used by Smart Car Sub Team, where it can output picture with various size and in various colour formats. The camera configuration is passed by a communication protocol called SCCB (similar to I2C), and it has also been included inside the camera library.<br>
Due to the limited memory size and computation power of STM32, the default image size is set to be 80 * 60.<br>
In this library, three colour formats are provided(RGB565, grey scale , black and white). You are also suggested to implement your own method with the raw data in RGB565 or grey scale.<br>
No path calculation algorithm will be provided, some hints from smart car seniors, however, will be given in tutorial 4b notes.<br>
Since this library is also a bit complex, there will only be introduction to part of the functions<br>
***As this is the first time of using it in the internal competition, please bear with me if there is any flaw in my code.***

#### Presetting
- Camera Library<br>
  1. Download ```camera.c```, ```camera.h``` and ```ov7725define.h```<br>
  2. Import the libraries to your project with the method mentioned above<br>
- Increasing the boundary of memory<br>
  1. Go to ```main.c```<br>
  2. Click ```Project > Option for Target 'stm32f103rb_template'...```<br>
  3. Change the ```Size``` for ```IRAM1``` from **0x5000** to **0xA000**<br>
  4. Click ```OK```<br>

#### cameraInit
```C
uint8_t cameraInit(ImageType outputType);
```
- **outputType** : the colour format of the output image, it can either be : <br>
```C
typedef enum
{
	RGBColour = 0,
	GreyScale,
	BlackNWhite,
	SelfDefine
}ImageType;
```
- **uint8_t return value** : ```SUCCESS``` will be returned if the initialization is successful; ```ERROR``` otherwise.

#### cameraTestTftDisplay
```C
void cameraTestTftDisplay(void);
```
The camera test function with tft output. Default as RGB565 output. Image will not be stored in the process.

#### cameraCaptureFrame
```C
uint8_t cameraCaptureFrame(void);
```
This function enable ov7725 to capture an image and store it in the FIFO buffer.<br>
It has to be called before ```cameraReceiveFrame```. 1ms of time interval is suggested between these 2 function.
- **uint8_t return value** : ```SUCCESS``` will be returned if image capture process is triggered successfully; ```ERROR``` otherwise.

#### cameraReceiveFrame
```C
uint8_t cameraReceiveFrame(void);
```
This function enable the mainboard to receive the image stored inside the FIFO buffer of the camera.<br>
It has to be called after ```cameraCaptureFrame```. 1ms of time interval is suggested between these 2 function.
- **uint8_t return value** : ```SUCCESS``` will be returned if image capture process is triggered successfully; ```ERROR``` otherwise.
- The image will be stored in
  - colourImage[][] : for ```RGBColour``` setting, where the colour format is RGB565
  - image[][] : for ```GreyScale``` setting, where the colour format is 8bit grey scale
  - image[][] : for ```BlackNWhite``` setting, where 1 is ```WHITE``` and 0 is ```BLACK```
- You may add ```extern uint16_t colourImage[ImageLength][ImageWidth];``` or ```extern uint8_t image[ImageLength][ImageWidth];``` to other library to handle the image there.<br>
***Remark : Please only use ```colourImage[][]``` or ```image[][]``` to avoid memory overflow***

#### Example
```C
//in front of main function
extern uint8_t image[ImageLength][ImageWidth];
//or
extern uint16_t colourImage[ImageLength][ImageWidth];
//inside while(1)
if(ticks % 10 == 1)
   cameraCaptureFrame();
if(ticks % 10 == 3)
   cameraReceiveFrame();
if(ticks % 20 == 11)
   /* handle captured image */
```

#### FIFO_READY
```C
FIFO_READY;
```
The macro function to trigger the FIFO output from the buffer of the camera to mainboard

#### READ_FIFO_COLOUR
```C
READ_FIFO_COLOUR(uint16_t);
```
The macro function for receiving the RGB image. It has to be called in a 2d for loop which would receive the pixel of the image from **left to right** and **top to bottom**.
- **uint16_t** : You can put the 16bit variable directly here to receive the data, no return value is needed

#### READ_FIFO_GREYSCALE
```C
READ_FIFO_GREYSCALE(uint8_t);
```
The macro function for receiving the grey scale image. It has to be called in a 2d for loop which would receive the pixel of the image from **left to right** and **top to bottom**.
- **uint8_t** : You can put the 8bit variable directly here to receive the data, no return value is needed

#### Example
```C
FIFO_READY;
for(uint16_t j=0;j<ImageLength;j++)
   for(uint16_t i=0;i<ImageWidth;i++)
      READ_FIFO_COLOUR(colourImage[j][i]);
      //or
      READ_FIFO_GREYSCALE(image[j][i]);
```

#### sccbWriteByte
```C
uint8_t sccbWriteByte(uint16_t address,uint8_t data);
```
The function to send command for configuring the camera via SCCB.<br>
- **address** : the address for the register in the camera, the list can be found in ov7725's schematic.<br>
Please try googling it by yourself
- **data** : the value to be set to the register, hence configuring the camera
- **uint8_t return value** : ```SUCCESS``` will be returned if the command has been sent successfully; ```Error``` otherwise.
