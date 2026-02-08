/*
 * base.h
 *
 *  Created on: 2026��2��6��
 *      Author: Elaina
 */

#ifndef DRIVER_BASE_H_
#define DRIVER_BASE_H_
#define PWM_MIN 500

#include "servo.h"
#include <lcd_play.h>
#include "debug.h"
#include "usart.h"
#include "gpio.h"
#include "AT24.h"
#include "timer.h"
#include "audio.h"
#include "picture.h"
#include "lcd.h"
#include "key.h"
#include "schedul.h"
#include "lcd_play.h"
#include "as608.h"
#include "esp8266.h"
#include "string.h"
  extern volatile unsigned long int uwtick;
  extern volatile unsigned long int as608_time;
  extern volatile unsigned long int uart7_rec_tick;
  extern volatile u8 uart7_index;
  extern u8 uart7_temp[30];
  extern u8 password_val[6];
  extern u8 key_val_index;
  extern u8 password_now[6];
  extern u8 fill_flag;
  extern u8 password_cmd[6];
  extern u8 mode;
  extern u8 password_error;
  extern u8 key_down,key_up;
  extern u8 servo_flag;
  extern u8 rfid_store_index;
  extern u8 rfid_temp[50];
  extern u8 uart6_rec_string[256];
  extern volatile u8 uart6_rec_tick;
  extern volatile u8 uart6_rec_index;
#endif /* DRIVER_BASE_H_ */


