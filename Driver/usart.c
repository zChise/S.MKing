/*
 * usart.c
 *
 *  Created on: 2026年2月6日
 *      Author: Elaina
 */
#include "base.h"

void UART3_Init(u32 baudrate)
{
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 先初始化GPIO
    UART3_GPIO_Init();

    // 配置UART3参数
    USART_InitStructure.USART_BaudRate = baudrate;                     // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // 收发模式

    USART_Init(USART3, &USART_InitStructure);

    // 使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能UART3
    USART_Cmd(USART3, ENABLE);
}

void UART2_Init(u32 baudrate)
{
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 先初始化GPIO
    UART2_GPIO_Init();

    // 配置UART3参数
    USART_InitStructure.USART_BaudRate = baudrate;                     // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // 收发模式

    USART_Init(USART2, &USART_InitStructure);

    // 使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能UART3
    USART_Cmd(USART2, ENABLE);
}

void UART7_Init(u32 baudrate)
{
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 先初始化GPIO
    UART7_GPIO_Init();

    // 配置UART3参数
    USART_InitStructure.USART_BaudRate = baudrate;                     // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   // 收发模式

    USART_Init(UART7, &USART_InitStructure);

    // 使能接收中断
    USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);

    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能UART3
    USART_Cmd(UART7, ENABLE);
}
