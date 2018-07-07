#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//
//STM32F407开发板
//LED驱动代码	   


//创建日期:2014/5/2
//版本：V1.0


////////////////////////////////////////////////////////////////////////////////// 	 

//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟

  //GPIO初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	

}






