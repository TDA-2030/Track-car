#include "stm32f4xx.h"
#include "PESensor_Track.h"
#include "delay.h"



void PhotoelectricSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|
	RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int8_t PhotoelectricSensor_Check(void)
{
	uint8_t result = 0;
	static int8_t i;
	//PC0~5、PB8~9
	result = (uint8_t)((~GPIOC->IDR & 0x003f)|(~GPIOB->IDR & 0x0300)>>2);
	
	for(i=0;i<8;i++)
	{
		if(result & (1<<i))
		{
			break;
		}
	}
	
	if(i==8) return 11;
	
	return i-4;
}

