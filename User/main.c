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
  volatile unsigned long int uwtick;
  volatile unsigned long int uart7_rec_tick;
  volatile unsigned long int as608_time;
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
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
u8 rfid_index = 0;
u8 rfid_verify[] = {0x04, 0x0C, 0x02, 0x30, 0x00, 0x04, 0x00};
u8 rfid_temp[50];
u8 rfid_val[6];
u8 rfid_store_index = 0;
u8 rfid_val_index;
u8 mode;  //mode = 2 --> rfid
u8 rfid_match_flag;
// 用软件延时产生PWM信号
u8 xieru = 1;
u8 z;
volatile u8 uart7_index;

u8 uart7_temp[30];
u8 as608_flaggg = 1;
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
    As608_Gpio_Init();
    UART7_Init(115200);
    UART2_Init(115200);
    audio_init();// usart3
    USART_ClearFlag(USART3, USART_FLAG_TC);
    audio_sound(20);
    audio_play(1);
    key_init();
    LCD_Init();
    TIM2_PWM_Init();
    Servo_Init();
    Tim3_Init(1000 - 1, 96 - 1);
    Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	USART_Printf_Init(115200);	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
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

	        if(rfid_match_flag == 1)  // 匹配成功
	        {
	            lcd_show_chinese(20,25,"门已开启",RED,WHITE,16,0);
//	            audio_play(2);  // 播放开锁音效
//	            // 舵机开锁逻辑
	            rfid_match_flag = 0;  // 清除标志
	        }

	        if(as608_flaggg == 1)
	        {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
                {
                    as608_play(1);
                    lcd_show_chinese(20,95,"中国",RED,WHITE,16,0);
                    as608_flaggg = 0;
                    Delay_Ms(1000);
                }
	        }
	        if(as608_flaggg == 0)
	        {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
                {
                    as608_search();

                }
	        }

    }
}






void TIM3_IRQHandler(void)
{
    // 检查是否是更新中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        uwtick++;
        uart7_rec_tick++;
        // 翻转标志位
        time_1000ms++;
        if(time_1000ms == 1000) time_1000ms = 0;
        // 清除中断标志位（重要！）
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}

void USART2_IRQHandler(void)
  {

      u8 temp = USART_ReceiveData(USART2);  // 读取一个字节
      //04 0C 02 30 00 04 00
      USART_ClearITPendingBit(USART2, USART_IT_RXNE);
      if(rfid_index <= 6)
      {
          if(temp == rfid_verify[rfid_index])
          {
             rfid_index++;
             return;
          }
          rfid_index = 0;
          return;
      }

      if(rfid_index == 7)
      {
          rfid_val[rfid_val_index] = temp;
          rfid_val_index++;
          if(rfid_val_index == 5)
             {
              if(xieru == 0)
              {
                  for(u8 j = 0; j <= rfid_store_index; j++)
                {
                    for(u8 i = j*5; i <= j*5 + 4; i++)
                    {
                       if(rfid_val[z] == rfid_temp[i]) z++;
                       if(z == 5) rfid_match_flag = 1;
                    }
                    z = 0;
                    }
                    rfid_index = 0;
                    rfid_val_index = 0;
              }
              if(xieru == 1)
              {
                  rfid_store_index++;
                  u16 zzz = 0;
                  for (i = rfid_store_index*5; i <= rfid_store_index*5 + 4 ; i++)
                  {
                    rfid_temp[i] = rfid_val[zzz];
                    zzz++;
                  }
                  lcd_show_chinese(20,25,"欢迎回家",RED,WHITE,16,0);
                  xieru = 0;
                  rfid_index = 0;
                  rfid_val_index = 0;
              }
             }
          return;
      }


    }


void UART7_IRQHandler(void)
{
    // 溢出错误：读SR再读DR清掉，否则中断卡死
    if(USART_GetFlagStatus(UART7, USART_FLAG_ORE) != RESET)
    {
        USART_ReceiveData(UART7);
        return;
    }

    if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
    {
        u8 temp;
        as608_time = uwtick;
        if(uart7_rec_tick>10) uart7_index=0;
        temp = USART_ReceiveData(UART7);
        uart7_temp[uart7_index] = temp;
        uart7_index++;
        uart7_rec_tick=0;
        USART_ClearITPendingBit(UART7, USART_IT_RXNE);
    }
}

