/*
 * base.h
 *
 *  Created on: 2026Äê2ÔÂ6ÈÕ
 *      Author: Elaina
 */

#ifndef DRIVER_BASE_H_
#define DRIVER_BASE_H_

#include <lcd_play.h>
#include "debug.h"
#include "usart.h"
#include "gpio.h"
#include "timer.h"
#include "audio.h"
#include "picture.h"
#include "lcd.h"
#include "key.h"
#include "schedul.h"
#include "lcd_play.h"
#include "as608.h"
  extern volatile unsigned long int uwtick;
  extern volatile unsigned long int as608_time;
  extern volatile unsigned long int uart7_rec_tick;
  extern volatile u8 uart7_index;
  extern u8 uart7_temp[30];
#endif /* DRIVER_BASE_H_ */


