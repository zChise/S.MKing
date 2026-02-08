/*
 * as608.c
 *
 *  Created on: 2026年2月7日
 *      Author: Elaina
 */
#include "base.h"
u8 as608_falg;
u8 old;
  void uart7_send_string(u8 *data, u16 len)
  {
      for(u16 i = 0; i < len; i++)
      {
          while(USART_GetFlagStatus(UART7, USART_FLAG_TXE) == RESET);
          USART_SendData(UART7, data[i]);
      }
      while(USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
  }

    void PS_GetImage()
    {
       u8 string[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x01,0x00,0x05};
       uart7_send_string(string,12);
    }

  void PS_GenChar(u8 buffer)
  {
      u8 string[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x04,0x02,buffer,0x00,0x01+0x00+0x04+0x02+ buffer};
      uart7_send_string(string,13);
  }

  void PS_RegModel()
  {
      u8 string[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x05,0x00,0x09};
      uart7_send_string(string,12);
  }
  void PS_StoreChar(u8 addr)
  {
      u8 string[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x06,0x06,0x02,0x00,addr,0x00,0x01+0x00+0x06+0x06+0x02+0x00+addr};
      uart7_send_string(string,15);

  }
  void PS_Search()
  {
      u8 string[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x08,0x04,0x02,0x00,0x00,0x00,0xff,0x01,0x0e};
       uart7_send_string(string,17);

  }
  void As608_Gpio_Init()
  {
      GPIO_InitTypeDef  GPIO_InitStructure;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

      GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
  void as608_wait(void)
    {
        u32 start = uwtick;
        uart7_index = 0;

        while(uart7_index == 0)
        {
            if(uwtick - start > 500) return;
        }

        while(uwtick - as608_time < 20)
        {
            if(uwtick - start > 2000) break;
        }
    }

void as608_play(u8 addr)
  {
    PS_GetImage();
    as608_wait();
    PS_GenChar(1);
    as608_wait();
    PS_GetImage();
    as608_wait();
    PS_GenChar(2);
    as608_wait();
    PS_RegModel();
    as608_wait();
    PS_StoreChar(addr);
    as608_wait();
  }

void as608_search()
{
    PS_GetImage();
    as608_wait();
    PS_GenChar(2);
    as608_wait();
    PS_Search();
    as608_wait();
    if(uart7_temp[13]>50)
    {
        lcd_show_chinese(20,15,"我爱你中国",RED,WHITE,16,0);
        servo_flag = 1;
        Servo_SetAngle(90);
    }
    else
    {
        lcd_show_chinese(20,15,"欢迎回家",RED,WHITE,16,0);
    }

}

void as608_proc()
{
    if(mode == 0)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
          {
              as608_search();
          }
    }
    if(mode == 7)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
          {
//              as608_play(2);
              lcd_show_chinese(20,95,"中国",RED,WHITE,16,0);
              mode = 0;
          }
    }
}
