/*
 * key.c
 *
 *  Created on: 2026年2月5日
 *      Author: Elaina
 */
#include "base.h"
u8 key_val,key_old,key_down,key_up;  //按键相关变量

void key_init()
  {
      // 配置行线1和2（PD11, PD9）为输出
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOD, &GPIO_InitStructure);

      // 配置行线3和4（PE15, PE13）为输出
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
      GPIO_Init(GPIOE, &GPIO_InitStructure);

      // 配置列线1,2,3（PE11, PE9, PE7）为输入
      // 这里时钟已经使能过了，不需要重复
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9 | GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
      GPIO_Init(GPIOE, &GPIO_InitStructure);

      // 配置列线4（PC5）为输入
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
      GPIO_Init(GPIOC, &GPIO_InitStructure);
  }

u8 key_read()
 {
     u8 temp=0;
     GPIO_ResetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=13;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=9;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=5;
     if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=1;

     GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_ResetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=14;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=10;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=6;
     if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=2;

     GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_ResetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=15;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=11;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=7;
     if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=3;

     GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_ResetBits(GPIOE, GPIO_Pin_13);
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=16;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=12;
     if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=8;
     if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=4;

     return temp;
 }
void key_proc()
{

    key_val=key_read();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;

    switch(key_down)
    {
    case 1:
        lcd_show_chinese(20,25,"欢迎回家",RED,WHITE,16,0);
        audio_play(1);
        break;
    case 2:
        lcd_show_chinese(20,25,"正在启动",RED,WHITE,16,0);
        audio_play(5);
        break;
    case 3:
    lcd_show_chinese(20,25,"我爱你中国",RED,WHITE,16,0);
        break;
    case 10:
        lcd_show_chinese(20,25,"门已开启",RED,WHITE,16,0);
        break;

    }
}
