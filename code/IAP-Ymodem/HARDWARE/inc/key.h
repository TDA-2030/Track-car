#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
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
/************ 	 端口定义 方便移植修改    ***************************/
/*  KEY1引脚  */
#define	 KEY1_Periph		 RCC_AHB1Periph_GPIOE
#define	 KEY1_Pin 	  	     GPIO_Pin_5
#define	 KEY1_GPIOx	  	     GPIOE
#define  KEY1				 PEin(5)
/*  KEY1引脚  */
#define	 KEY2_Periph		 RCC_AHB1Periph_GPIOE
#define	 KEY2_Pin 	  	     GPIO_Pin_6
#define	 KEY2_GPIOx	  	     GPIOE
#define  KEY2				 PEin(6)

#define KEY_LEFT	1
#define KEY_RIGHT	2

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
