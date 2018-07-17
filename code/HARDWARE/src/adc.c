#include "adc.h"
#include "delay.h"		 
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


//初始化ADC
//开启ADC1_CH8 	ADC1_CH9
void  Adc_Init(void)
{    
	//先初始化IO口
 	RCC->APB2ENR|=1<<8;    	//使能ADC1时钟 高电平使能
	RCC->AHB1ENR|=1<<1;    	//使能PORTB时钟	即GPIOB
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB0,模拟输入,不上下拉
	GPIO_Set(GPIOB,PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB1,模拟输入,不上下拉
	
	RCC->APB2RSTR|=1<<8;   	//ADCs复位。0-31中第8位，1复位，0保持
	RCC->APB2RSTR&=~(1<<8);	//ADCs退出复位状态，不然ADC_DR等寄存器一直处于复位状态	 
	ADC->CCR=3<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 8分频
	ADC->CCR|=1<<23;		//使能内部温度传感器
 	
	ADC1->CR1=0;   			//CR1设置清零
	ADC1->CR2=0;   			//CR2设置清零
	ADC1->CR1|=0<<24;      	//12位模式
	ADC1->CR1|=0<<8;    	//非扫描模式	
	
	ADC1->CR2&=~(1<<1);    	//单次转换模式，单次转换
 	ADC1->CR2&=~(1<<11);   	//右对齐
	ADC1->CR2|=0<<28;    	//软件触发，禁止外部触发
	
	ADC1->SQR1&=~(0XF<<20); //先清零再写值
	ADC1->SQR1|=1<<20;     	//2个转换在规则序列中 也就是只转换规则序列1和2			   
	//设置通道8，9的采样时间
	ADC1->SMPR2&=~(7<<(3*8));//通道8采样时间清空
	ADC1->SMPR2&=~(7<<(3*9));//通道9采样时间清空
 	ADC1->SMPR2|=7<<(3*8); 	//通道8  480个周期,提高采样时间可以提高精确度	
	ADC1->SMPR2|=7<<(3*9); 	//通道9  480个周期,提高采样时间可以提高精确度
 	ADC1->CR2|=1<<0;	   	//开启AD转换器	  
}


//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<30;       //启动规则转换通道 
	while(!(ADC1->SR&1<<1));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
float Get_Adc_Average(u8 ch,u8 times)
{
	static float last_val1,last_val2;
	
	float temp_val=0;
	
	temp_val = (float)(Get_Adc(ch))*2.5/40.96;
	if(ch==8)
	{
		last_val1 = last_val1*0.92+temp_val*0.08;
		return last_val1;
	}
	else if(ch==9)
	{
		last_val2 = last_val2*0.8+temp_val*0.2;
		return last_val2;
	}
}  
//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=Get_Adc_Average(ADC_CH_TEMP,20);	//读取通道16,20次取平均
	temperate=(float)adcx*(3.3/4096);		//电压值
	temperate=(temperate-0.76)/0.0025+25; 	//转换为温度值 
	result=temperate*=100;					//扩大100倍.
	return result;
}






