#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tft_display.h"
#include "key.h"  
#include "malloc.h" 
#include "sdio_sdcard.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"
#include "touch.h"
#include "rtc.h"
#include "pwm.h"
#include "beep.h"
#include "adc.h"
#include "timer.h"

#include "PESensor_Track.h"

#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#include "string.h"	


void System_Init(u16 x,u16 y);

volatile uint32_t SysTime=0;

typedef struct {
	uint16_t current,voltage;
	uint8_t state;
}System_TypeDef;

typedef struct {
	int8_t pes;
	uint8_t state;
}Sensor_TypeDef;


enum{
	SYSTEM_STATE_IDLE,
	SYSTEM_STATE_RUN,
	SYSTEM_STATE_ERROR,
};


System_TypeDef SystemInfo={0};
Sensor_TypeDef SensorInfo={0};


int main(void)
{
	u16 t,toll=0;
	uint32_t simple_time=0;
	
	float pitch,roll,yaw; 	//欧拉角
	short temp;		        //温度
	int8_t pes;
	
	System_Init(0,3);
	
	if(RTC_Read_BKR(18) == 0x43215678 && RTC_Read_BKR(19) == 0x12348765)  //按键按下
	{
		while(1)Set_Beep(100,10);
	}
	
	Set_Beep(100,10);
	Set_Beep(100,11);
	Set_Beep(100,12);
	SystemInfo.state=SYSTEM_STATE_IDLE;
	
	while(1)
	{
		delay_ms(8);
		
		if(USART_RX_STA&0x4000)
		{
			if((USART_RX_STA&0X3FFF) == 4&&
				USART_RX_BUF[0]=='w'&&
				USART_RX_BUF[1]=='d'&&
				USART_RX_BUF[2]=='z'&&
				USART_RX_BUF[3]=='d')
			{
				Set_Beep(50,13);Set_Beep(50,10);
				RTC_Write_BKR(18,0x43215678);
				RTC_Write_BKR(19,0x12348765);
				Sys_Soft_Reset();
			}
			else if((USART_RX_STA&0X3FFF) == 2&&
				USART_RX_BUF[0]=='t'&&
				USART_RX_BUF[1]=='t')
			{
				toll=0;
				Motor_Speed(1,toll);
				SystemInfo.state = SYSTEM_STATE_IDLE;
				Set_Beep(300,4);
			}
			else if((USART_RX_STA&0X3FFF) == 4&&
				USART_RX_BUF[0]=='k'&&
				USART_RX_BUF[1]=='k')
			{
				toll = (((USART_RX_BUF[2]-48)*10)+(USART_RX_BUF[3]-48));
				SystemInfo.state = SYSTEM_STATE_RUN;
				Set_Beep(300,8);
			}
			else
			{
				toll=0;
				Motor_Speed(1,toll);
				SystemInfo.state = SYSTEM_STATE_IDLE;
				Set_Beep(30,4);
			}
			
			USART_RX_STA=0;
		}
		
		if(SysTime>simple_time)
		{
			simple_time = SysTime+10;
			SystemInfo.current = Get_Adc_Average(8,20)*100/11;
			SystemInfo.voltage = Get_Adc_Average(9,3)*(float)5.666;
			if(((SystemInfo.current>100) || (SystemInfo.voltage<1110)) && (SysTime>8000))
			{
				toll=0;
				Motor_Speed(1,toll);
				SystemInfo.state = SYSTEM_STATE_ERROR;
			}
		}
		
		LCD_ShowNum(20,5,SystemInfo.current,4,16);
		LCD_ShowNum(80,5,SystemInfo.voltage,4,16);
		LCD_ShowNum(30,40,toll,3,24);
		
		if(SystemInfo.state == SYSTEM_STATE_IDLE)
		{
			if(PEN==0)
			{
				tp_dev.scan(0);
				LCD_DrawPoint(tp_dev.x[0],tp_dev.y[0]);
				LCD_DrawPoint(tp_dev.x[0]+1,tp_dev.y[0]+1);
				toll = tp_dev.y[0]/10;
			}
			
			
			if(mpu_mpl_get_data(&pitch,&roll,&yaw)==0)
			{
				temp=MPU_Get_Temperature();	//得到温度值
				
				if((++t%5)==0)
				{
					if(temp<0)
					{
						LCD_ShowChar(30+48,200,'-',16,0);		//显示负号
						temp=-temp;		//转为正数
					}else LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
					LCD_ShowNum(30+48+8,200,temp/100,3,16);		//显示整数部分	    
					LCD_ShowNum(30+48+40,200,temp%10,1,16);		//显示小数部分 
					temp=pitch*10;
					if(temp<0)
					{
						LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
						temp=-temp;		//转为正数
					}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
					LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
					LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 
					temp=roll*10;
					if(temp<0)
					{
						LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
						temp=-temp;		//转为正数
					}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
					LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
					LCD_ShowNum(30+48+40,240,temp%10,1,16);		//显示小数部分 
					temp=yaw*10;
					if(temp<0)
					{
						LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
						temp=-temp;		//转为正数
					}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
					LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
					LCD_ShowNum(30+48+40,260,temp%10,1,16);		//显示小数部分  
					t=0;
				}
			}
		}
		else if(SystemInfo.state == SYSTEM_STATE_RUN)
		{
			Motor_Speed(1,toll);
			
			pes=PhotoelectricSensor_Check();LCD_ShowNum(30,70,abs(pes),3,24);
			if(pes<10)
			{
				Servor_Turn_Angle(TURN_SERVO_FORWARD,pes*8);
				if(abs(pes)>1)
					Servor_Turn_Angle(TURN_SERVO_BACKWARD,-pes*8);
				else
					Servor_Turn_Angle(TURN_SERVO_BACKWARD,0);
			}
			else
			{
				Servor_Turn_Angle(TURN_SERVO_FORWARD,0);
				Servor_Turn_Angle(TURN_SERVO_BACKWARD,0);
			}
		}
		
	}
}





#define BLUETOOTH_CONFIG 0
#define APP_SCB_BASE  0XC000  //中断向量表偏移量,不可随意修改

void System_Init(u16 x,u16 y)
{
	u32 CpuID[3];
	char buf[30];
	uint16_t back_point_color = POINT_COLOR;
	uint16_t back_back_color = BACK_COLOR;
	
	SCB->VTOR=FLASH_BASE | APP_SCB_BASE;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	BEEP_Init();
	KEY_Init();					//按键初始化 
	delay_init(168);            //初始化延时函数
	#if BLUETOOTH_CONFIG
	uart_init(84,38400);delay_ms(1000);
	printf("AT+UART=115200,0,0\r\n");Set_Beep(100,10);
	while(1);
	#endif
	uart_init(84,115200);		    //初始化串口波特率为115200
	
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化
	LCD_ShowString(x,y,200,16,16,"system start...");y+=20;
	TIM1_PWM_Init();
	TIM2_PWM_Init();
	TIM4_PWM_Init();
	TIM5_PWM_Init();
	TIM3_Int_Init(999,83);
	PhotoelectricSensor_Init();
	Adc_Init();
	
	BACK_COLOR = BLACK;
	POINT_COLOR = WHITE;
	LCD_Clear(BACK_COLOR);
	
	LCD_ShowString(x,y,200,16,16,"TOUCH init");
	if(0 == tp_dev.init())				//初始化触摸屏
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	LCD_ShowString(x,y,200,16,16,"RTC init");
	if(0 == MyRTC_Init())				//初始化RTC
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	LCD_ShowString(x,y,200,16,16,"SPI FLASH init");
	if(0 == W25QXX_Init())				//初始化W25Q128
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();			//为fatfs相关变量申请内存 
	
	LCD_ShowString(x,y,200,16,16,"SD CARD mount");
	if(FR_OK == f_mount(fs[0],"0:",1))				//挂载SD卡
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	LCD_ShowString(x,y,200,16,16,"SPI FLASH mount");
	if(FR_OK == f_mount(fs[1],"1:",1))				//挂载FLASH.
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	LCD_ShowString(x,y,200,16,16,"MPU9250 init");
	if(0 == MPU_Init() && 0==mpu_dmp_init()) //初始化MPU9250陀螺仪        	
	{
		LCD_ShowString(x+170,y,40,16,16,"ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x+170,y,40,16,16,"fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	if(KEY0==0&&KEY1==0) TP_Adjust();  	//屏幕校准 
	
	//获取CPU唯一ID
	CpuID[0]=*(vu32*)(0x1fff7a10);
	CpuID[1]=*(vu32*)(0x1fff7a14);
	CpuID[2]=*(vu32*)(0x1fff7a18);
	/* printf the chipid */
	sprintf(buf,"%X-%X-%X",CpuID[0],CpuID[1],CpuID[2]);
	LCD_ShowString(x,y,240,16,16,"Chip ID:");
	LCD_ShowString(x+64,y,240,16,16,(u8*)buf);
	y+=16;
	
	LCD_ShowString(x,y,240,16,16,__DATE__);
	y+=16;
	LCD_ShowString(x,y,240,16,16,__TIME__);
	y+=16;
	delay_ms(300);
	
	BACK_COLOR = back_back_color;
	POINT_COLOR = back_point_color;
	
	LCD_Clear(BACK_COLOR);
//	POINT_COLOR=RED;      
//	while(font_init()) 		//检查字库
//	{
//		LCD_ShowString(30,50,200,16,16,"Font Error!");
//		delay_ms(200);
//		LCD_Fill(30,50,240,66,WHITE);//清除显示
//		delay_ms(200);
//	}
}









