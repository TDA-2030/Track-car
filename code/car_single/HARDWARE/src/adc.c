#include "adc.h"
#include "delay.h"		 


#define ADC_BUFFER_SIZE  30

static uint16_t AdcBuffer[ADC_BUFFER_SIZE]={0};


//初始化ADC
//开启ADC1_CH8 	ADC1_CH9
void  Adc_Init(void)
{   
	//先初始化IO口
 	RCC->APB2ENR|=1<<8;    	//使能ADC1时钟 高电平使能
	RCC->AHB1ENR|=1<<1;    	//使能PORTB时钟	即GPIOB
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB0,模拟输入,不上下拉
	GPIO_Set(GPIOB,PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB1,模拟输入,不上下拉
	
	RCC->AHB1ENR|=1<<22;//DMA2时钟使能 
	
	DMA2_Stream0->CR = 0;
	delay_us(10);
	DMA2_Stream0->CR |= 0<<25;
	DMA2_Stream0->CR |= 0<<23;//存储器突发传输配置
	DMA2_Stream0->CR |= 0<<21;//外设突发传输配置
	DMA2_Stream0->CR |= 0<<19;
	DMA2_Stream0->CR |= 0<<18;
	DMA2_Stream0->CR |= 1<<16;//中优先级
	DMA2_Stream0->CR |= 0<<15;
	DMA2_Stream0->CR |= 1<<13;
	DMA2_Stream0->CR |= 1<<11;
	DMA2_Stream0->CR |= 1<<10;
	DMA2_Stream0->CR |= 0<<9;
	DMA2_Stream0->CR |= 1<<8;
	DMA2_Stream0->CR |= 0<<6;
	DMA2_Stream0->CR |= 0<<5;
	
	DMA2_Stream0->M0AR = (u32)AdcBuffer;
	DMA2_Stream0->PAR  = (u32)&ADC1->DR;
	DMA2_Stream0->NDTR = ADC_BUFFER_SIZE;
	
	DMA2_Stream0->CR |= 1<<0; //开启DMA
	
	RCC->APB2RSTR|=1<<8;   	//ADCs复位。0-31中第8位，1复位，0保持
	RCC->APB2RSTR&=~(1<<8);	//ADCs退出复位状态，不然ADC_DR等寄存器一直处于复位状态	 
	ADC->CCR=3<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 8分频
	ADC->CCR|=1<<23;		//使能内部温度传感器
 	
	ADC1->CR1=0;   			//CR1设置清零
	ADC1->CR2=0;   			//CR2设置清零
	
	ADC1->CR1|=0<<24;      	//12位模式
	ADC1->CR1|=1<<8;    	//非扫描模式
	
	ADC1->CR2|=1<<8;	   	//开启DMA
	ADC1->CR2|=1<<9;
	ADC1->CR2|=1<<1;    	//连续转换模式
	
	ADC1->SQR1&=~(0X0F<<20); //先清零再写值
	ADC1->SQR1|=1<<20;     	//2个转换在规则序列中 也就是只转换规则序列1和2			   
	//设置采样时间
	ADC1->SMPR2&=~(7<<24);//通道8采样时间清空
	ADC1->SMPR2|=7<<24; 	
	ADC1->SMPR2&=~(7<<27);//通道9采样时间清空
	ADC1->SMPR2|=7<<27; 
	ADC1->SMPR1&=~(7<<18);//通道16采样时间清空
	ADC1->SMPR1|=7<<18; 
	
	ADC1->SQR3&=0XFFFF6000;//规则序列1 通道ch
	ADC1->SQR3|=((8<<5)|(9<<0)|(ADC_CH_TEMP<<10));
	
 	ADC1->CR2|=1<<0;	   	//开启AD转换器
	ADC1->CR2|=1<<30;       //启动规则转换通道
}



//获取通道ch的转换值，取times次,然后平均
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
float Get_Adc_Average(u8 ch,u8 times)
{
	static float last_val1,last_val2;
	
	float temp_val;
	
	if(ch==8)
	{
		temp_val = (float)(AdcBuffer[1])*2.5/40.96;
		last_val1 = last_val1*0.92+temp_val*0.08;
		return last_val1;
	}
	else if(ch==9)
	{
		temp_val = (float)(AdcBuffer[0])*2.5/40.96;
		last_val2 = last_val2*0.8+temp_val*0.2;
		return last_val2;
	}
	return 0;
}

//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=AdcBuffer[2];	//读取通道16,20次取平均
	temperate=(float)adcx*(2.5/4096);		//电压值
	temperate=(temperate-0.76)/0.0025+25; 	//转换为温度值 
	result=temperate*=100;					//扩大100倍.
	return result;
}






