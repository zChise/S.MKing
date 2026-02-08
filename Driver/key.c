/*
 * key.c
 *
 *  Created on: 2026年2月5日
 *      Author: Elaina
 */
#include "base.h"
u8 key_val,key_old,key_down,key_up;  //按键相关变量
u8 key_val_index;
u8 password_flag;
u8 fill_flag = 1;
u8 password_cmd[6] = {1,9,1,9,8,1};
u8 password_error;
u8 servo_flag;
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

void key_clear()
{
    memset(password_val,10,6);
    key_val_index=0;
    fill_flag = 1;
}
void key_proc()
{

    password_flag = 0;
    key_val=key_read();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;

    if(password_error >= 3) return;

    switch(key_down)
    {
    case 1:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 1;
        key_val_index++;
        break;
    case 2:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 2;
        key_val_index++;
        break;
    case 3:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 3;
        key_val_index++;
        break;
    case 4:
        mode = 1;
        key_clear();

        break;
    case 5:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 4;
        key_val_index++;
        break;
    case 6:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 5;
        key_val_index++;
        break;
    case 7:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 6;
        key_val_index++;
        break;
    case 8:
         mode = 3;
        break;
    case 9:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 7;
        key_val_index++;
        break;
    case 10:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 8;
        key_val_index++;
        break;
    case 11:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 9;
        key_val_index++;
        break;
    case 12:
        mode = 2;
        key_clear();
        break;
    case 13:
        key_clear();
        break;
    case 14:
        if(key_val_index == 6) break;
        password_val[key_val_index] = 0;
        key_val_index++;
        break;
    case 15:
        if(key_val_index >= 1)
        {
            key_val_index--;
            password_val[key_val_index] = 10;
            fill_flag = 1;
        }
        break;
    case 16:
        if (mode == 0) {
            for (u8 i = 0; i < 6; i++)
            {
                if(password_val[i] == password_now[i]) password_flag++;
            }
            if(password_flag == 6)
                {
                    lcd_show_chinese(20,50,"外星人电解水",RED,WHITE,16,0);
                    Delay_Ms(200);
                    password_error = 0;
                    Servo_SetAngle(90);
                    servo_flag = 1;
                }

            if(password_flag != 6)
                {
                    lcd_show_chinese(20,50,"门已开启",RED,WHITE,16,0);
                     Delay_Ms(200);
                     password_error++;
                }
        }

        if (mode == 1) {
            for (u8 i = 0; i < 6; i++)
            {
                if(password_val[i] == password_cmd[i]) password_flag++;
            }
                if(password_flag == 6)
                    {
                        lcd_show_chinese(20,50,"中国人",RED,WHITE,16,0);
                        mode = 5;
                        password_error = 0;
                    }
                if(password_flag != 6)
                    {
                        lcd_show_chinese(20,50,"水星人",RED,WHITE,16,0);
                        password_error++;
                    }
                key_clear();
                return;
        }

        if (mode == 2) {
            for (u8 i = 0; i < 6; i++)
            {
                if(password_val[i] == password_cmd[i]) password_flag++;
            }
                if(password_flag == 6)
                    {
                        lcd_show_chinese(20,50,"中国人",RED,WHITE,16,0);
                        mode = 6;
                        password_error = 0;
                    }
                if(password_flag != 6)
                    {
                        lcd_show_chinese(20,50,"水星人",RED,WHITE,16,0);
                        password_error++;
                    }
                key_clear();
                return;
        }
        if (mode == 3) {
            for (u8 i = 0; i < 6; i++)
            {
                if(password_val[i] == password_cmd[i]) password_flag++;
            }
                if(password_flag == 6)
                    {
                        lcd_show_chinese(20,50,"中国人",RED,WHITE,16,0);
                        mode = 7;
                        password_error = 0;
                    }
                if(password_flag != 6)
                    {
                        lcd_show_chinese(20,50,"水星人",RED,WHITE,16,0);
                        password_error++;
                    }
                key_clear();
                return;
        }
            if (mode == 5) {
                for (u8 i = 0; i < 6; i++)
                {
                    password_now[i] = password_val[i];
                }
                lcd_show_chinese(20,50,"开水门",RED,WHITE,16,0);
                mode = 0;
            }


        key_clear();
        break;
    }

}
