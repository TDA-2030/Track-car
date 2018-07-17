#include "beep.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	



const u8 tone_cycle[14]={191,170,152,143,128,114,101,96,85,76,72,64,57,51}; //单位：10微秒



//time:响的时间，单位是毫秒
//tone：音调，1到14
void Set_Beep(u16 time,u8 tone)
{
	u16 period_back = tone_cycle[tone-1]; //
	u16 period = period_back;

	time = 100*time/period_back;
	while(time--)
	{
		BEEP=0;
		while(--period)	
			{delay_us(5);}
		period = period_back;
		
		BEEP=1;
		while(--period)	
			{delay_us(5);}
		period = period_back;
	}
	BEEP=1;
}


//初始化PF8为输出口
//BEEP IO初始化
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOF时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;  //PF8引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //速度100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //推完输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;   //下拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13); //PF8拉低
}



