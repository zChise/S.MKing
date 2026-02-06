/*
 * timer.c
 *
 *  Created on: 2026年2月6日
 *      Author: Elaina
 */
#include "base.h"
#define PWM_MIN 500
#define PWM_PERIOD 20000
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




