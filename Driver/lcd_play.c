/*
 * lcd_play.c
 *
 *  Created on: 2026年2月6日
 *      Author: Elaina
 */

#include "base.h"

void lcd_proc()
{
 u8 i = key_val_index;
 if(fill_flag == 1)
 {
     LCD_Fill(16,45,112,66,YELLOW);//填充黄色背景
     fill_flag = 0;
 }

   while(i--)
   {
       LCD_ShowChar(20+16*i,45,'*',RED,YELLOW,16,0);
   }
   LCD_ShowIntNum(5, 5, mode, 1, RED, WHITE, 16);
}
