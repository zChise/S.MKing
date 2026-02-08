/*
 * as608.h
 *
 *  Created on: 2026Äê2ÔÂ7ÈÕ
 *      Author: Elaina
 */

#ifndef DRIVER_AS608_H_
#define DRIVER_AS608_H_

void PS_GetImage();
void PS_GenChar(u8 buffer);
void PS_RegModel();
void PS_StoreChar(u8 addr);
void PS_Search();
void As608_Gpio_Init();
void as608_search();
void as608_play(u8 addr);
void as608_wait();
void as608_proc();
#endif /* DRIVER_AS608_H_ */
