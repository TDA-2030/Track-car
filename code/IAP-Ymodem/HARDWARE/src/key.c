#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(KEY1_Periph|KEY2_Periph, ENABLE);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//普通输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉	
	//=====KEY1
	GPIO_InitStructure.GPIO_Pin = KEY1_Pin;	
	GPIO_Init(KEY1_GPIOx, &GPIO_InitStructure);					//初始化GPIO
	//=====KEY2
	GPIO_InitStructure.GPIO_Pin = KEY2_Pin;	
	GPIO_Init(KEY2_GPIOx, &GPIO_InitStructure);					//初始化GPIO	
	
//	GPIO_SetBits();	
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&((KEY1==1)||(KEY2==1)))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==1)return 1;
		else if(KEY2==1)return 2;
	}else if((KEY1==0)&&(KEY2==0))key_up=1; 	    
 	return 0;// 无按键按下
}

