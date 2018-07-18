#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define LEFT 1
#define RIGHT 2
#define TURN_SERVO_FORWARD 3
#define TURN_SERVO_BACKWARD 4

void TIM5_PWM_Init(void);
void Servor_Act_Angle(u8,s8 );
void TIM4_PWM_Init(void);
void Light_Set(float,u8 );
void TIM2_PWM_Init(void);
void Servor_Turn_Angle(u8,s8);
void TIM1_PWM_Init(void);
void Motor_Speed(u8 num,int8_t v);

#endif

