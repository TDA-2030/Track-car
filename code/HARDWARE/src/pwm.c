#include "pwm.h"
#include "led.h"


//PWM输出初始化函数
//舵机，探照灯，电机控制函数


//控制人体动作舵机初始化函数 频率50HZ
void TIM5_PWM_Init()//PA输出PWM
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0|GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3,GPIO_AF_TIM5); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM5的通道1、2、3、4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化
	
	TIM_TimeBaseStructure.TIM_Prescaler=143;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=9999;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 Channel PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;  //初始化占空比为0
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5OC1
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCRx上的预装载寄存器
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
 										  
}  

//人体动作控制舵机角度设置函数
void Servor_Act_Angle(u8 num,s8 angle)//num:通道编号
{
	 float CCR,Pulse;//比较值与高电平时间
	 Pulse=(angle+135)/90;
	 CCR=500*Pulse;
	 
	 switch(num)
	 {
			case 1:TIM_SetCompare1(TIM5,(u32)CCR);break;
			case 2:TIM_SetCompare2(TIM5,(u32)CCR);break;
			case 3:TIM_SetCompare3(TIM5,(u32)CCR);break;
			case 4:TIM_SetCompare4(TIM5,(u32)CCR);break;
	 }
}

//前照灯PWM输出初始化2KHZ
void TIM4_PWM_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	 
	 GPIO_PinAFConfig(GPIOD,GPIO_PinSource12|GPIO_PinSource13,GPIO_AF_TIM4);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; //TIM4的通道1、2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	 GPIO_Init(GPIOD,&GPIO_InitStructure); 
	 
	 TIM_TimeBaseStructure.TIM_Prescaler=71;  //定时器分频
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	 TIM_TimeBaseStructure.TIM_Period=499;   //自动重装载值
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	 
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	 TIM_OCInitStructure.TIM_Pulse = 0;  //占空比为0
	 TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC1、OC2
	 TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	 
	 TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCRx上的预装载寄存器
	 TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	 
	 TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	
	 TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

//前照灯亮度调节函数
void Light_Set(float per,u8 fx)//per:占空比 fx(方向):前照灯位置
{
   float width,CCR;
	 width =0.5f*per;
	 CCR=1000*width;
	 switch(fx)
	 {
			case LEFT :TIM_SetCompare1(TIM4,(u16)CCR);break;
			case RIGHT:TIM_SetCompare2(TIM4,(u16)CCR);break;
	 }
}


void TIM2_PWM_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource10|GPIO_PinSource11,GPIO_AF_TIM2);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //TIM4的通道1、2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	 GPIO_Init(GPIOB,&GPIO_InitStructure); 
	 
	 TIM_TimeBaseStructure.TIM_Prescaler=143;  //定时器分频
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	 TIM_TimeBaseStructure.TIM_Period=9999;   //自动重装载值
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	 
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	 TIM_OCInitStructure.TIM_Pulse = 0;  //占空比为0
	 TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2OC1、OC2
	 TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	 
	 TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCRx上的预装载寄存器
	 TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	 
	 TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能 
	
	 TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}

//转弯角度控制舵机角度设置函数
void Servor_Turn_Angle(u8 fx,s8 angle)//fx:舵机方向
{
	 float CCR,Pulse;//比较值与高电平时间
	 Pulse=(angle+135)/90;
	 CCR=500*Pulse;
	 switch(fx)
	 {
			case FORWARD :TIM_SetCompare3(TIM5,(u32)CCR);break;
			case BACKWARD:TIM_SetCompare4(TIM5,(u32)CCR);break;
	 }
}


//电机控制PWM输出初始化
void TIM1_PWM_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	 
	 GPIO_PinAFConfig(GPIOE,GPIO_PinSource9|GPIO_PinSource11,GPIO_AF_TIM1);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11; //TIM1的通道1、2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	 GPIO_Init(GPIOE,&GPIO_InitStructure); 
	 
	 TIM_TimeBaseStructure.TIM_Prescaler=143;  //定时器分频
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	 TIM_TimeBaseStructure.TIM_Period=9999;   //自动重装载值
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	 
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	 TIM_OCInitStructure.TIM_Pulse = 0;  //占空比为0
	 TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1OC1、OC2
	 TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	 
	 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCRx上的预装载寄存器
	 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	 
	 TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	 
	 TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	 TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}

//电机速度控制函数
void Motor_Speed(u8 num,float time)//time:高电平时间
{
   float CCR;//比较值
	 CCR=500*time;
	 switch(num)
	 {
			case 1 :TIM_SetCompare1(TIM1,(u16)CCR);break;
			case 2 :TIM_SetCompare2(TIM1,(u16)CCR);break;
	 }
}

