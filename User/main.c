/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "picture.h"
#include "lcd.h"
/* Global typedef */

/* Global define */

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
#define PWM_MIN 500
#define PWM_PERIOD 20000
u16 i = 0;
void TIM2_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // 使能TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // ====== 配置定时器基本参数 ======
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD - 1;  // 自动重装载值 19999
    TIM_TimeBaseStructure.TIM_Prescaler = 96 - 1;       // 预分频器 95（96MHz→1MHz）
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ====== 配置PWM模式 ======
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;         // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 输出使能
    TIM_OCInitStructure.TIM_Pulse = PWM_MIN;                  // 初始脉宽（0度）
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 高电平有效

    // 配置TIM2通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能预装载

    // 使能自动重装载预装载
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

u16 time_1000ms;
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void LED_Test_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // 改成GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // 改成Pin_2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  // 改成GPIOB
}

void Tim3_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // ====== 配置定时器基本参数 ======
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;              // 时钟分割（通常设为0）
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_Period = arr;                   // 自动重装载值
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;                // 预分频值

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // 使能更新中断和触发中断
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

    // ====== 配置NVIC中断优先级 ======
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              // TIM3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 使能中断

    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}
// 用软件延时产生PWM信号

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 改成GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // 改成Pin_2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // 改成GPIOB
}

void Servo_SetAngle(uint16_t angle)
{
    uint16_t pulse;

    // 角度限幅
    if(angle > 180) angle = 180;

    // 角度转脉宽
    pulse = PWM_MIN + (angle * 2000 / 180);

    // 设置比较值（修改占空比）
    TIM_SetCompare1(TIM2, pulse); //20ms
}

int main(void)
{
    LCD_Init();
    TIM2_PWM_Init();
    Servo_Init();
    Tim3_Init(1000 - 1, 96 - 1);
    Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	USART_Printf_Init(115200);	
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");
	LED_Test_Init();
    LCD_Fill(0,0,128,128,WHITE);//整个屏幕填充白色
    LCD_ShowPicture(0,0,127,127,gImage_hutao);
    lcd_show_chinese(20,50,"外星人电解水",RED,WHITE,16,0);//开机显示


    for(i = 0; i<= 128; i++)
    {
        LCD_DrawLine(0,80,i,80,BLACK);
        LCD_DrawLine(0,81,i,81,BLACK);
        LCD_DrawLine(0,82,i,82,BLACK);
        LCD_DrawLine(0,83,i,83,BLACK);
        LCD_DrawLine(0,84,i,84,BLACK);
        Delay_Ms(10);
    }

	while(1)
    {
	    if(time_1000ms <= 500)
	    {
	        Servo_SetAngle(90);
//	        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//            Delay_Ms(20);
//            GPIO_SetBits(GPIOA, GPIO_Pin_0);
//            Delay_Us(1500);
//	        GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	    }
	    if(time_1000ms > 500)
	    {
	        Servo_SetAngle(0);
//	        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//	        Delay_Ms(20);
//	        GPIO_SetBits(GPIOA, GPIO_Pin_0);
//	        Delay_Us(500);
//	        GPIO_SetBits(GPIOC, GPIO_Pin_1);
	    }

	}
}


void TIM3_IRQHandler(void)
{
    // 检查是否是更新中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        // 翻转标志位
        time_1000ms++;
        if(time_1000ms == 1000) time_1000ms = 0;
        // 清除中断标志位（重要！）
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}



