#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									   
//********************************************************************************
//20140507 	V1.1
//1,修改Adc_Init函数,支持内部温度传感器数据采集.
//2,新增Get_Temprate函数,用于获取内部温度传感器采集到的温度值
//20140714  V1.2
//1,新增Adc3_Init函数,用于初始化ADC3
//2,新增Get_Adc3函数,用于读取ADC3的某个通道
////////////////////////////////////////////////////////////////////////////////// 


#define ADC_CH5  		5 		 	//通道5	   	      	    
#define ADC_CH_TEMP  	16 		 	//通道16,内部温度传感器专用通道	   	    
	   									   
void Adc_Init(void); 				//ADC初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
float Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
short Get_Temprate(void);			//读取内部温度传感器值 
void Adc3_Init(void); 				//ADC3初始化
u16  Get_Adc3(u8 ch); 				//获得ADC3某个通道值  
#endif 















