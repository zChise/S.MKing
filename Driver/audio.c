/*
 * audio.c
 *
 *  Created on: 2026Äê2ÔÂ6ÈÕ
 *      Author: Elaina
 */
#include "base.h"

  void audio_init() {
      UART3_Init(9600);
  }

  void audio_play(u8 num)
  {
     u8 string[]={0x7e,0x05,0x41,0x00,num,0x05^0x41^0x00^num,0xef};
     u8 i;
     for(i=0;i<7;i++)
     {
         USART_SendData(USART3,string[i]);
         while(  USART_GetFlagStatus(USART3, USART_FLAG_TC) == 0 );
     }
  }

  void audio_sound(u8 num)
  {
     u8 string[]={0x7e,0x04,0x31,num,0x04^0x31^num,0xef};
     u8 i;
     for(i=0;i<6;i++)
     {
         USART_SendData(USART3,string[i]);
         while(  USART_GetFlagStatus(USART3, USART_FLAG_TC) == 0 );
     }
  }
