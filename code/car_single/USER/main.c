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

#include "PESensor_Track.h"

#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#include "string.h"	



void System_Init(u16 x,u16 y);


int main(void)
{
	u16 t,toll=50;
	
	float pitch,roll,yaw; 	//欧拉角
	short aacx,aacy,aacz;	//加速度传感器原始数据
	short gyrox,gyroy,gyroz;//陀螺仪原始数据 
	short temp;		        //温度
	int8_t pes;
	
	System_Init(0,3);
	
	Set_Beep(100,10);
	Set_Beep(100,11);
	Set_Beep(100,13);
	
	while(1)
	{
		if(PEN==0)
		{
			tp_dev.scan(0);
			LCD_DrawPoint(tp_dev.x[0],tp_dev.y[0]);
			LCD_DrawPoint(tp_dev.x[0]+1,tp_dev.y[0]+1);
			toll = 50+tp_dev.y[0]/36;
			pes = (tp_dev.x[0]-120)/9.23;
		}
		
		if(mpu_mpl_get_data(&pitch,&roll,&yaw)==0)
        {
			GPIOE->ODR ^= GPIO_Pin_7;
            temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//  			if(report)mpu9250_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//发送加速度+陀螺仪原始数据
//			if(report)usart1_report_imu((int)(roll*100),(int)(pitch*100),(int)(yaw*100),0,0);
			if((t%10)==0)
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
				LED0=!LED0;//DS0闪烁
			}
		}
		
//		Servor_Act_Angle(1,-5);Servor_Act_Angle(2,-15);Servor_Act_Angle(3,-25);Servor_Act_Angle(4,-35);
//		Servor_Turn_Angle(3,-20);Servor_Turn_Angle(4,-15);
//		delay_ms(800);
//		Servor_Act_Angle(1,35);Servor_Act_Angle(2,25);Servor_Act_Angle(3,15);Servor_Act_Angle(4,5);
//		Servor_Turn_Angle(3,20);Servor_Turn_Angle(4,15);
//		delay_ms(800);
		
		if(KEY0==0&&toll>1)toll--; 
		if(KEY1==0&&toll<100)toll++; 
		Motor_Speed(1,toll);
		
		LCD_ShowNum(30,20,Get_Adc_Average(8,20)*100/11,4,16);
		LCD_ShowNum(30,60,Get_Adc_Average(9,3)*5.666,4,16);
		LCD_ShowNum(30,100,(u32)Get_Temprate(),4,16);
		LCD_ShowNum(30,120,toll,4,16);
		
	
		//pes=PhotoelectricSensor_Check();
		if(pes<10)
			Servor_Turn_Angle(3,pes*6);
		else
		Servor_Turn_Angle(3,0);
		
		
		delay_ms(8);
	}
}



void System_Init(u16 x,u16 y)
{
	u32 CpuID[3];
	char buf[30];
	uint16_t back_point_color = POINT_COLOR;
	uint16_t back_back_color = BACK_COLOR;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	BEEP_Init();
	KEY_Init();					//按键初始化 
	delay_init(168);            //初始化延时函数
	uart_init(115200);		    //初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化
	LCD_ShowString(x,y,200,16,16,"system start...");y+=20;
	TIM1_PWM_Init();
	TIM2_PWM_Init();
	TIM4_PWM_Init();
	TIM5_PWM_Init();
	PhotoelectricSensor_Init();
	Adc_Init();
	
	BACK_COLOR = BLACK;
	POINT_COLOR = WHITE;
	LCD_Clear(BACK_COLOR);
	if(0 == tp_dev.init())				//初始化触摸屏
	{
		LCD_ShowString(x,y,200,16,16,"TOUCH init ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"TOUCH init fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	if(0 == MyRTC_Init())				//初始化RTC
	{
		LCD_ShowString(x,y,200,16,16,"RTC init ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"RTC init fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	if(0 == W25QXX_Init())				//初始化W25Q128
	{
		LCD_ShowString(x,y,200,16,16,"SPI FLASH init ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"SPI FLASH init fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();			//为fatfs相关变量申请内存 
	
	if(FR_OK == f_mount(fs[0],"0:",1))				//挂载SD卡
	{
		LCD_ShowString(x,y,200,16,16,"SD CARD mount ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"SD CARD mount fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	if(FR_OK == f_mount(fs[1],"1:",1))				//挂载FLASH.
	{
		LCD_ShowString(x,y,200,16,16,"SPI FLASH mount ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"SPI FLASH mount fail");
		POINT_COLOR=back_point_color;
		delay_ms(200);
	}
	y+=16;
	
	if(0 == MPU_Init())				//初始化MPU9250陀螺仪
	{
		LCD_ShowString(x,y,200,16,16,"MPU9250 init ok");
	}
	else
	{
		POINT_COLOR=RED;
		LCD_ShowString(x,y,200,16,16,"MPU9250 init fail");
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
	LCD_ShowString(x+64,y,240,16,16,buf);
	
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









