/*
 * esp8266.h
 *
 *  Created on: 2026Äê2ÔÂ8ÈÕ
 *      Author: Elaina
 */

#ifndef DRIVER_ESP8266_H_
#define DRIVER_ESP8266_H_

#include "debug.h"

void uart6_send_string(u8 *data, u16 len);
void usart1_send_string(u8 *data, u16 len);
void esp8266_init(void);
void esp8266_proc(void);
void mqtt_publish_raw(char* json);

#endif /* DRIVER_ESP8266_H_ */
