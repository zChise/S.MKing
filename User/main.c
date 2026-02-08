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

#include "base.h"

  volatile unsigned long int uwtick;
  volatile unsigned long int uart7_rec_tick;
  volatile unsigned long int as608_time;

#define PWM_PERIOD 20000
u16 i = 0;
u16 time_1000ms;
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
u8 rfid_index = 0;
u8 rfid_verify[] = {0x04, 0x0C, 0x02, 0x30, 0x00, 0x04, 0x00};
u8 rfid_temp[50];
u8 rfid_val[6];
u8 rfid_store_index;
u8 rfid_val_index;
u8 mode;  // 0:Ö÷Ò³ 1:cmd 2:rfid 5:ÐÞ¸ÄÃÜÂë 6:Â¼Èë¿¨Æ¬
u8 rfid_match_flag;
u8 xieru = 1;
u32 time15000;
volatile u8 uart7_index;
u8 uart7_temp[30];
u8 as608_flaggg = 1;
u8 password_now[6];
u8 password_val[6] = {10,10,40,50,10,40};
u8 z;
u16 time_3000;
u16 time_5000;
u8 key_flag;
u8 uart6_rec_string[256] = {0};
volatile u8 uart6_rec_tick;
volatile u8 uart6_rec_index;
u8 at_rst[]="AT+RST\r\n";
u8 at_cwmode[]="AT+CWMODE=1\r\n";
u8 at_dhcp[]="AT+CWDHCP=1,1\r\n";
u8 at_cwjap[]="AT+CWJAP=\"Chise\",\"Ryougi Shiki\"\r\n";
u8 at_mqttcfg[]="AT+MQTTUSERCFG=0,1,\"ch32\",\"3139b5EoDM\",\"version=2018-10-31&res=products%2F3139b5EoDM%2Fdevices%2Fch32&et=1770790216&method=md5&sign=SUuJuZqRLHacHI91QDReig%3D%3D\",0,0,\"\"\r\n";
u8 at_mqttconn[]="AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n";
u8 at_mqttsub1[]="AT+MQTTSUB=0,\"$sys/3139b5EoDM/ch32/thing/property/set\",1\r\n";
u8 at_mqttsub2[]="AT+MQTTSUB=0,\"$sys/3139b5EoDM/ch32/thing/property/desired/set\",1\r\n";


int main(void)
{
    AT24C02_Init();
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


	EEPROM_Read(password_now,0x00,6);
	EEPROM_Read(&rfid_store_index,0x07,1);
	EEPROM_Read(rfid_temp,0x10,50);

    LCD_Fill(0,0,128,128,WHITE);
    LCD_ShowPicture(0,0,127,127,gImage_hutao);
    lcd_show_chinese(20,50,"ÕýÔÚÆô¶¯",RED,WHITE,16,0);

    esp8266_init();
    {
        u8 ii = 0;
        while(ii < 128)
        {
            LCD_DrawLine(ii, 100, ii, 128, RED);
            Delay_Ms(100);
            ii++;
            switch(ii)
            {
                case 1:  uart6_send_string(at_rst, sizeof(at_rst)-1); break;
                case 10: uart6_send_string(at_cwmode, sizeof(at_cwmode)-1); break;
                case 20: uart6_send_string(at_dhcp, sizeof(at_dhcp)-1); break;
                case 30: uart6_send_string(at_cwjap, sizeof(at_cwjap)-1); break;
                case 50: uart6_send_string(at_mqttcfg, sizeof(at_mqttcfg)-1); break;
                case 70: uart6_send_string(at_mqttconn, sizeof(at_mqttconn)-1); break;
                case 90: uart6_send_string(at_mqttsub1, sizeof(at_mqttsub1)-1); break;
                case 110: uart6_send_string(at_mqttsub2, sizeof(at_mqttsub2)-1); break;
            }
        }
    }
    uart6_rec_index = 0;
    memset(uart6_rec_string, 0, 256);

    LCD_ShowPicture(0,0,128,128,gImage_2);
    LCD_Fill(16,45,112,66,YELLOW);
    Servo_SetAngle(0);
    scheduler_init();

	while(1)
    {
	    scheduler_run();
    }
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(password_error == 3)
        {
            time15000++;
            if(time15000 >= 15000)
                {
                    password_error = 2;
                    time15000 = 0;
                }
        }
        if(servo_flag == 1)
        {
            time_5000++;
            if(time_5000 >= 5000)
                {
                    Servo_SetAngle(0);
                    time_5000 = 0;
                    servo_flag = 0;
                }

        }
        if(key_down == 15) key_flag = 1;
        if(key_up == 15)
        {
            key_flag = 0;
            time_3000 = 0;
        }

        if(key_flag == 1)
        {
            time_3000++;
        }
        if(time_3000 >= 3000)
        {
            if(mode == 1) mode = 0;
            if(mode == 2) mode = 0;
            if(mode == 3) mode = 0;
            if(mode == 6) mode = 0;
            key_flag = 0;
            time_3000 = 0;
            key_clear();
        }

        uwtick++;
        uart7_rec_tick++;
        uart6_rec_tick++;
        time_1000ms++;
        if(time_1000ms == 1000) time_1000ms = 0;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }

}

void USART2_IRQHandler(void)
  {

      u8 temp = USART_ReceiveData(USART2);
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
              z = 0;
              if(mode == 0)
              {
                  for(u8 j = 0; j <= rfid_store_index; j++)
                {
                    for(u8 i = j*5; i <= (j*5 + 4); i++)
                    {
                       if(rfid_val[z] == rfid_temp[i]) z++;

                       }
                    if(z == 5)
                          {
                              servo_flag = 1;
                              Servo_SetAngle(90);

                              rfid_index = 0;
                              rfid_val_index = 0;
                              lcd_show_chinese(20,50,"Ë¢¿¨³É¹¦",RED,WHITE,16,0);
                              return;
                          }

                          if(z != 5)
                          {
                              lcd_show_chinese(20,50,"Ë¢¿¨Ê§°Ü",RED,WHITE,16,0);


                    }
                   z = 0;
                    rfid_index = 0;
                    rfid_val_index = 0;
              }
              }
              if(mode == 6)
              {
                  rfid_store_index++;
                  u16 zzz = 0;
                  for (i = rfid_store_index*5; i <= rfid_store_index*5 + 4 ; i++)
                  {
                    rfid_temp[i] = rfid_val[zzz];
                    zzz++;
                  }
                  mode = 0;
                  rfid_index = 0;
                  rfid_val_index = 0;
              }
             }
          return;
      }


    }


void UART7_IRQHandler(void)
{
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

void UART6_IRQHandler(void)
{
    u8 temp = 0;
    if(USART_GetITStatus(UART6, USART_IT_RXNE) != RESET)
    {
        uart6_rec_tick = 0;
        temp = USART_ReceiveData(UART6);
        uart6_rec_string[uart6_rec_index] = temp;
        uart6_rec_index++;
    }
    USART_ClearITPendingBit(UART6, USART_IT_RXNE);
}
