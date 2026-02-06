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

#include "base.h"
/* Global typedef */

/* Global define */

/* Global Variable */
unsigned long int uwtick;

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
u16 time_1000ms;
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));



// 用软件延时产生PWM信号



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

    audio_init();
    USART_ClearFlag(USART3, USART_FLAG_TC);
    audio_sound(10);
    key_init();
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

    scheduler_init();
//    for(i = 0; i<= 128; i++)
//    {
//        LCD_DrawLine(0,80,i,80,BLACK);
//        LCD_DrawLine(0,81,i,81,BLACK);
//        LCD_DrawLine(0,82,i,82,BLACK);
//        LCD_DrawLine(0,83,i,83,BLACK);
//        LCD_DrawLine(0,84,i,84,BLACK);
//        Delay_Ms(10);
//    }



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
	    scheduler_run();
	}
}






void TIM3_IRQHandler(void)
{
    // 检查是否是更新中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        uwtick++;
        // 翻转标志位
        time_1000ms++;
        if(time_1000ms == 1000) time_1000ms = 0;
        // 清除中断标志位（重要！）
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}



