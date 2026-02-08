/*
 * AT24.h
 *
 *  Created on: 2026Äê2ÔÂ8ÈÕ
 *      Author: Elaina
 */

#ifndef DRIVER_AT24_H_
#define DRIVER_AT24_H_

void w_24cXX(unsigned char slave_add,unsigned char byte_add,unsigned char byte_add2,unsigned char dat);
unsigned char r_24cXX(unsigned char slave_add,unsigned char byte_add,unsigned char byte_add2);
void AT24C02_Init();
void EEPROM_Write(u8* string,u8 addr,u8 len);
void EEPROM_Read(u8* string,u8 addr,u8 len);
void AT24_proc();

#endif /* DRIVER_AT24_H_ */
