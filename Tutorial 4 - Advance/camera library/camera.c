/*	
 *	Camera library - internal 2017/18
 *	Mode 		: ov7725
 *  Author  : Emmett Yim
 *  Contact : yhyim@ust.hk 
 *						66816823
 *	Lib			: camera.c
 */

#include "camera.h"

#define ImageLength 60
#define ImageWidth 80

ImageType imageType = RGBColour;
uint8_t camState = 0;
uint8_t threshold = 0;
uint8_t image[ImageLength][ImageWidth];
uint16_t colourImage[ImageLength][ImageWidth];

//register default
OV7725REG cameraConfig[] =
{
	{CLKRC,     0x00},
	{COM7,      0x46},
	//default value for QVGA
	//{HSTART,    0x3f},
	//{HSIZE,     0x50},
	//{VSTRT,     0x03},
	//{VSIZE,     0x78},
	//{HREF,      0x00},
	//tuned value for 80*60	
	{HSTART,    0x20},
	{HSIZE,     ImageWidth>>2},
	{VSTRT,     0x2f},
	{VSIZE,     ImageLength>>1},
	{HREF,      (ImageLength&0x01<<2)|(ImageWidth&0x03)},
	{HOutSize,  ImageWidth>>2},
	{VOutSize,  ImageLength>>1},
	{EXHCH,     (ImageLength&0x01<<2)|(ImageWidth&0x03)},
	//{HOutSize,  0x50},
	//{VOutSize,  0x78},
	//{EXHCH,     0x00},
	{TGT_B,     0x7f},
	{FixGain,   0x09},
	{AWB_Ctrl0, 0xe0},
	{DSP_Ctrl1, 0xff},
	{DSP_Ctrl2, 0x20},
	{DSP_Ctrl3,	0x00},
	{DSP_Ctrl4, 0x00},
	{COM8,		  0xf0},
	{COM4,		  0x81},
	{COM6,		  0xc5},
	{COM9,		  0x21},
	{BDBase,	  0xFF},
	{BDMStep,	  0x01},
	{AEW,		    0x34},
	{AEB,		    0x3c},
	{VPT,		    0xa1},
	{EXHCL,		  0x00},
	{AWBCtrl3,  0xaa},
	{COM8,		  0xff},
	{AWBCtrl1,  0x5d},
	{EDGE1,		  0x0a},
	{DNSOff,	  0x01},
	{EDGE2,		  0x01},
	{EDGE3,		  0x01},
	{MTX1,		  0x5f},
	{MTX2,		  0x53},
	{MTX3,		  0x11},
	{MTX4,		  0x1a},
	{MTX5,		  0x3d},
	{MTX6,		  0x5a},
	{MTX_Ctrl,  0x1e},
	{BRIGHT,	  0x00},
	{CNST,		  0x25},
	{USAT,		  0x65},
	{VSAT,		  0x65},
	{UVADJ0,	  0x81},
	{SDE,		    0x06},
	{GAM1,		  0x0c},
	{GAM2,		  0x16},
	{GAM3,		  0x2a},
	{GAM4,		  0x4e},
	{GAM5,		  0x61},
	{GAM6,		  0x6f},
	{GAM7,		  0x7b},
	{GAM8,		  0x86},
	{GAM9,		  0x8e},
	{GAM10,		  0x97},
	{GAM11,		  0xa4},
	{GAM12,		  0xaf},
	{GAM13,		  0xc5},
	{GAM14,		  0xd7},
	{GAM15,		  0xe8},
	{SLOP,		  0x20},
	{HUECOS,	  0x80},
	{HUESIN,	  0x80},
	{DSPAuto,	  0xff},
	{DM_LNL,	  0x00},
	{BDBase,	  0x99},
	{BDMStep,	  0x03},
	{LC_RADI,	  0x00},
	{LC_COEF,	  0x13},
	{LC_XC,		  0x08},
	{LC_COEFB,  0x14},
	{LC_COEFR,  0x17},
	{LC_CTR,	  0x05},
	{COM3,		  0xd0},
	{COM5,			0xf5},
};

uint8_t cameraConfigLength = sizeof(cameraConfig)/sizeof(cameraConfig[0]);

//sccb
uint8_t cameraSccbInit(ImageType type)
{
	//sccb gpio config
	//	SCL - PA0
	//	SDA - PA1
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//init config
	uint8_t ov7725ID = 0;
	if(!sccbWriteByte(COM7,0x80))
		return ERROR ;
	if(!sccbReadByte( &ov7725ID,1,PID))	
		return ERROR;
	if(ov7725ID != OV7725ID_PID)
		return ERROR;
	if(!sccbReadByte( &ov7725ID,1,VER))	
		return ERROR;
	if(ov7725ID != OV7725ID_VER)
		return ERROR;
	for(uint16_t i=0;i<cameraConfigLength;i++)
		if(!sccbWriteByte(cameraConfig[i].address,cameraConfig[i].data))
			return ERROR;
	if(type == GreyScale || type == BlackNWhite)
	{
		//8bit greyscale - capture with image, recommended
		sccbWriteByte(COM7,0x40);
		//16bit greyscale - capture with colourImage
		//sccbWriteByte(SDE,0x26);
		//sccbWriteByte(UFix,0x80);
		//sccbWriteByte(VFix,0x80);
	}
	
	/* self define camera config here
	 * some useful config is also included
	 * you may also call the function
	 * 		sccbWriteByte(RegisterAddress,Value);
	 * in main.c to tune the value in the while loop
	 */
	if(type == SelfDefine)
	{
		// -image colour, resolution setting-
		//sccbWriteByte(COM7,);
		
		// -image size setting-
		//sccbWriteByte(HSTART,);
		//sccbWriteByte(HSIZE,);
		//sccbWriteByte(VSTRT,);
		//sccbWriteByte(VSIZE,);
		//sccbWriteByte(HREF,);
		//sccbWriteByte(HOutSize,);
		//sccbWriteByte(VOutSize,);
		//sccbWriteByte(EXHCH,);
		
		// -digital effect-
		//sccbWriteByte(SDE,);
		
		// -YUV setting-
		//sccbWriteByte(UFix,);
		//sccbWriteByte(VFix,);
		
		// -Brightness-
		//sccbWriteByte(BRIGHT,);
		
		// -Contrast-
		//sccbWriteByte(CNST,);
	}
	
	return SUCCESS;
}

static void sccbDelay(void)
{	
   uint16_t i = 400; 
   while(i) i--;
}

static uint8_t sccbStart(void)
{
	SDA_H;
	SCL_H;
	sccbDelay();
	if(!SDA_read)
	return DISABLE;
	SDA_L;
	sccbDelay();
	if(SDA_read) 
	return DISABLE;
	SDA_L;
	sccbDelay();
	return ENABLE;
}

static void sccbStop(void)
{
	SCL_L;
	sccbDelay();
	SDA_L;
	sccbDelay();
	SCL_H;
	sccbDelay();
	SDA_H;
	sccbDelay();
}

static void sccbAck(void)
{	
	SCL_L;
	sccbDelay();
	SDA_L;
	sccbDelay();
	SCL_H;
	sccbDelay();
	SCL_L;
	sccbDelay();
}

static void sccbNoAck(void)
{	
	SCL_L;
	sccbDelay();
	SDA_H;
	sccbDelay();
	SCL_H;
	sccbDelay();
	SCL_L;
	sccbDelay();
}

static uint8_t sccbWaitAck(void) 	
{
	SCL_L;
	sccbDelay();
	SDA_H;			
	sccbDelay();
	SCL_H;
	sccbDelay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

static void sccbSendByte(uint8_t SendByte) 
{
	uint8_t i = 8;
	while(i--)
	{
		SCL_L;
		sccbDelay();
		if(SendByte & 0x80)
			SDA_H;  
		else 
			SDA_L;   
		SendByte <<= 1;
		sccbDelay();
		SCL_H;
		sccbDelay();
	}
	SCL_L;
}

static uint8_t sccbReceiveByte(void)  
{ 
    uint8_t i = 8;
    uint8_t receiveByte = 0;
    SDA_H;				
    while(i--)
    {
      receiveByte <<= 1;      
      SCL_L;
      sccbDelay();
			SCL_H;
      sccbDelay();	
      if(SDA_read)
        receiveByte |= 0x01;
    }
    SCL_L;
    return receiveByte;
}

uint8_t sccbWriteByte(uint16_t address,uint8_t data)
{
	if(!sccbStart())
		return DISABLE;
	sccbSendByte(OV7725ADR);
	if(!sccbWaitAck())
	{
		sccbStop(); 
		return DISABLE;
	}
	sccbSendByte((uint8_t)(address & 0x00FF));     
	sccbWaitAck();	
	sccbSendByte(data);
	sccbWaitAck();   
	sccbStop(); 
	return ENABLE;
}

uint8_t sccbReadByte(uint8_t* buffer, uint16_t length, uint8_t address)
{
	if(!sccbStart())
		return DISABLE;
	sccbSendByte(OV7725ADR);
	if(!sccbWaitAck())
	{
		sccbStop(); 
		return DISABLE;
	}
	sccbSendByte(address);
	sccbWaitAck();
	sccbStop();
	if(!sccbStart())
		return DISABLE;
	sccbSendByte( OV7725ADR + 1 );
	if(!sccbWaitAck())
	{
		sccbStop(); 
		return DISABLE;
	}
	while(length)
	{
		*buffer = sccbReceiveByte();
		if(length == 1)
			sccbNoAck();
		else
			sccbAck(); 
		buffer++;
		length--;
	}
	sccbStop();
	return ENABLE;
}

//ov7725
uint8_t cameraInit(ImageType type)
{
	//check image type
	if(type != RGBColour && type != GreyScale && type != BlackNWhite)
		return ERROR;
	//sccb init
	if(!cameraSccbInit(type))
		return ERROR;
	//fifo init
	//	GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//		OE - PA15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//		RRST - PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//		RCLK - PB1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//		WRST - PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//		WEN - PC4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//		Data - PB8 to PB15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//		VSYNC - PB0
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//	NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//	EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_GenerateSWInterrupt(EXTI_Line0);
	//camera gpio init
	FIFO_OE_L();
	FIFO_WEN_H();
	//camera status init
	camState = 0;
	//set camera image type
	imageType = type;
	return SUCCESS;
}

uint8_t cameraReceiveFrame(void)
{
	if(camState != 2)
		return ERROR;
	FIFO_READY;
	if(imageType == RGBColour)
		for(uint16_t j=0;j<ImageLength;j++)
			for(uint16_t i=0;i<ImageWidth;i++)
				READ_FIFO_COLOUR(colourImage[j][i]);
	else if(imageType == GreyScale)
		for(uint16_t j=0;j<ImageLength;j++)
			for(uint16_t i=0;i<ImageWidth;i++)
				READ_FIFO_GREYSCALE(image[j][i]);
	else if(imageType == BlackNWhite)
	{
		uint32_t sum = 0;
		for(uint16_t j=0;j<ImageLength;j++)
			for(uint16_t i=0;i<ImageWidth;i++)
			{
				READ_FIFO_GREYSCALE(image[j][i]);
				sum += image[j][i];
			}
		//greyscale to black n white
		threshold = sum / ImageLength / ImageWidth;
		for(uint16_t j=0;j<ImageLength;j++)
			for(uint16_t i=0;i<ImageWidth;i++)
			{
				if(image[j][i] >= threshold)
					image[j][i] = 1;
				else
					image[j][i] = 0;
			}
		//end
	}
	else//if(imageType == SelfDefine)
	{
		//your own implementation
	}
	return SUCCESS;
}

uint8_t cameraCaptureFrame(void)
{
	if(camState != 2)
		return ERROR;
	camState = 0;
	return SUCCESS;
}

inline uint16_t GreyScale8bitTo16bit(uint8_t input)
{
	return ((input << 8) & 0xF800) | ((input << 3) & 0x07E0) | (input >> 3);
}

void cameraTestTftDisplay(void)
{
	static uint8_t init=0;
	if(!init)
	{
		while(!cameraInit(RGBColour));
		init = 1;
	}
	if(camState == 2)
	{
		FIFO_READY;
		tft_clear();
		uint32_t t=get_ticks();
		for(uint16_t j=0;j<ImageLength;j++)
			for(uint16_t i=0;i<ImageWidth;i++)
			{
				//RGB
				READ_FIFO_COLOUR(colourImage[j][i]);
				tft_put_pixel(i+16,j+24,colourImage[j][i]);
			}
		tft_prints(0,9,"%d",get_ticks() - t);
		tft_update();
		camState = 0;
	}
}

void EXTI0_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line0) != RESET )
	{
		if(camState == 0)
		{
			FIFO_WRST_L();
			FIFO_WEN_L();
			camState = 1;	   	
			FIFO_WEN_H();
			FIFO_WRST_H();
		}
		else if(camState == 1)
		{
			FIFO_WEN_L();
			camState = 2;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
