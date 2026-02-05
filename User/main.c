/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*
* Git版本控制演示 - 这是第二次提交的修改
*
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
void TIM2_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // ʹ��TIM2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // ====== ���ö�ʱ���������� ======
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD - 1;  // �Զ���װ��ֵ 19999
    TIM_TimeBaseStructure.TIM_Prescaler = 96 - 1;       // Ԥ��Ƶ�� 95��96MHz��1MHz��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ====== ����PWMģʽ ======
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;         // PWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // ���ʹ��
    TIM_OCInitStructure.TIM_Pulse = PWM_MIN;                  // ��ʼ������0�ȣ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // �ߵ�ƽ��Ч

    // ����TIM2ͨ��1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // ʹ��Ԥװ��

    // ʹ���Զ���װ��Ԥװ��
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // ������ʱ��
    TIM_Cmd(TIM2, ENABLE);
}

u16 time_1000ms;
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void LED_Test_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // �ĳ�GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // �ĳ�Pin_2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  // �ĳ�GPIOB
}

void Tim3_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // ʹ��TIM3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // ====== ���ö�ʱ���������� ======
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;              // ʱ�ӷָͨ����Ϊ0��
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���ģʽ
    TIM_TimeBaseInitStruct.TIM_Period = arr;                   // �Զ���װ��ֵ
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;                // Ԥ��Ƶֵ

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // ʹ�ܸ����жϺʹ����ж�
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

    // ====== ����NVIC�ж����ȼ� ======
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              // TIM3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          // ��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // ʹ���ж�

    NVIC_Init(&NVIC_InitStructure);

    // ������ʱ��
    TIM_Cmd(TIM3, ENABLE);
}
// ��������ʱ����PWM�ź�

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // �ĳ�GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // �ĳ�Pin_2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // �ĳ�GPIOB
}

void Servo_SetAngle(uint16_t angle)
{
    uint16_t pulse;

    // �Ƕ��޷�
    if(angle > 180) angle = 180;

    // �Ƕ�ת����
    pulse = PWM_MIN + (angle * 2000 / 180);

    // ���ñȽ�ֵ���޸�ռ�ձȣ�
    TIM_SetCompare1(TIM2, pulse); //20ms
}
int main(void)
{
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
    // ����Ƿ��Ǹ����ж�
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        // ��ת��־λ
        time_1000ms++;
        if(time_1000ms == 1000) time_1000ms = 0;
        // ����жϱ�־λ����Ҫ����
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}



