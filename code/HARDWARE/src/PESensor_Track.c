#include "stm32f4xx.h"
#include "PESensor_Track.h"
#include "delay.h"


#define OUT1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define OUT2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define OUT3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define OUT4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
#define OUT5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define OUT6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define OUT7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define OUT8  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)


void PhotoelectricSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|
	RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

unsigned char PhotoelectricSensor_Check(void)
{
	int result = 0;
	if(OUT1 == 0 |OUT2 == 0 |OUT3 == 0 |OUT4 == 0 |OUT5 == 0 |OUT6 == 0 |OUT7 == 0 |OUT8 == 0)
	{
		delay_ms(1);
		if(OUT1 == 0)
			result += 0x80;
		if(OUT2 == 0)
			result += 0x40;
		if(OUT3 == 0)
			result += 0x20;
		if(OUT4 == 0)
			result += 0x10;
		if(OUT5 == 0)
			result += 0x08;
		if(OUT6 == 0)
			result += 0x04;
		if(OUT7 == 0)
			result += 0x02;
		if(OUT8 == 0)
			result += 0x01;
	}
	return result;
}

