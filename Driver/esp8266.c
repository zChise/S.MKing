/*
 * esp8266.c
 *
 *  Created on: 2026Äê2ÔÂ8ÈÕ
 *      Author: Elaina
 */
#include "base.h"
#include "stdio.h"

void uart6_send_string(u8 *data, u16 len)
{
    for(u16 i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(UART6, USART_FLAG_TXE) == RESET);
        USART_SendData(UART6, data[i]);
    }
    while(USART_GetFlagStatus(UART6, USART_FLAG_TC) == RESET);
}

void usart1_send_string(u8 *data, u16 len)
{
    for(u16 i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, data[i]);
    }
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void esp8266_init(void)
{
    UART6_Init(115200);
}

void mqtt_publish_raw(char* json)
{
    u16 len = strlen(json);
    char cmd[128];
    sprintf(cmd, "AT+MQTTPUBRAW=0,\"$sys/3139b5EoDM/ch32/thing/property/post\",%d,0,0\r\n", len);
    uart6_send_string((u8*)cmd, strlen(cmd));
    Delay_Ms(200);
    uart6_send_string((u8*)json, len);
}

void esp8266_proc(void)
{
    if(uart6_rec_index == 0) return;
    if(uart6_rec_tick > 10)
    {
        usart1_send_string(uart6_rec_string, uart6_rec_index);

        char* add1 = strstr((char*)uart6_rec_string, "appkey");
        if(add1 == 0) { uart6_rec_index = 0; return; }
        int appkey = 0;
        sscanf(add1, "appkey\":%d", &appkey);

        char* add2 = strstr((char*)uart6_rec_string, "appstring");
        int appstring = 0;
        if(add2) sscanf(add2, "appstring\":\"%d", &appstring);

        switch(appkey)
        {
            case 1:
                servo_flag = 1;
                Servo_SetAngle(90);
                audio_play(3);
                break;

            case 2:
                Servo_SetAngle(0);
                servo_flag = 0;
                audio_play(2);
                break;

            case 3:
                password_now[0] = appstring / 100000 % 10;
                password_now[1] = appstring / 10000 % 10;
                password_now[2] = appstring / 1000 % 10;
                password_now[3] = appstring / 100 % 10;
                password_now[4] = appstring / 10 % 10;
                password_now[5] = appstring % 10;
                EEPROM_Write(password_now, 0x00, 6);
                mqtt_publish_raw("{\"id\":\"123\",\"params\":{\"appkey\":{\"value\":6}}}");
                break;

            case 4:
                audio_sound((u8)appstring);
                mqtt_publish_raw("{\"id\":\"123\",\"params\":{\"appkey\":{\"value\":7}}}");
                break;

            case 7:
                mqtt_publish_raw("{\"id\":\"123\",\"params\":{\"appkey\":{\"value\":0}}}");
                break;

            case 8:
                mqtt_publish_raw("{\"id\":\"123\",\"params\":{\"appkey\":{\"value\":0}}}");
                break;
        }

        uart6_rec_index = 0;
    }
}
