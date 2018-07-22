#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "tft_display.h"
#include "key.h"   
#include "stmflash.h" 
#include "iap.h"   
#include "menu.h"
#include "beep.h"

pFunction Jump_To_Application;


//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		Jump_To_Application=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		for(int i = 0; i < 8; i++)
		{			
			NVIC->ICER[i] = 0xFFFFFFFF;	/* 关闭中断*/
			NVIC->ICPR[i] = 0xFFFFFFFF;	/* 清除中断标志位 */
		}
		Jump_To_Application();									//跳转到APP.
	}
}	

//RTC写入后备区域SRAM
//BKPx:后备区寄存器编号,范围:0~19
//data:要写入的数据,32位长度
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(u32*)temp)=data; 
}
//RTC读取后备区域SRAM
//BKPx:后备区寄存器编号,范围:0~19
//返回值:读取到的数据
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(u32*)temp);		//返回读取到的值
}

void Serial_PutString(uint8_t *s);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200

	LED_Init();					//初始化LED
	LCD_Init();					//LCD初始化
	KEY_Init();					//按键初始化 
	BEEP_Init();
	LCD_Clear(BACK_COLOR);
	POINT_COLOR=BLUE;    //设置字体为红色 
	LCD_ShowString(30,30,200,16,16, "Bootload with Ymodem");	
	
	LCD_ShowString(30,46,240,16,16,__DATE__);
	LCD_ShowString(30,62,240,16,16,__TIME__);
	
	POINT_COLOR=RED;//设置字体为蓝色	 
	
	Set_Beep(200,6);
	
	RCC->APB1ENR|=1<<28;			//使能电源接口时钟
	PWR->CR|=1<<8;					//后备区域访问使能(RTC+SRAM)
	
	if(PEin(5)==0 || (RTC_Read_BKR(18) == 0x43215678 && RTC_Read_BKR(19) == 0x12348765))  //按键按下
	{
		if(Main_Menu()==1)
		{
			
		}
		RTC_Write_BKR(18,0);
		RTC_Write_BKR(19,0);
	}
	LCD_ShowString(30,150,200,16,16,"enter app");
	iap_load_app(APPLICATION_ADDRESS);
	LCD_ShowString(30,150,200,16,16,"can not enter app!!!");
	while(1)
	{
		LCD_ShowString(30,150,200,16,16,"                    ");Set_Beep(200,2);
		LCD_ShowString(30,150,200,16,16,"can not enter app!!!");Set_Beep(200,2);
		RTC_Write_BKR(18,0x43215678);
		RTC_Write_BKR(19,0x12348765);
		Serial_PutString("can not enter app!!!\r");
		Sys_Soft_Reset();
	}  
}

