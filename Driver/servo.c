
#include "base.h"
void Servo_SetAngle(uint16_t angle)
{
    uint16_t pulse;

    // 角度限幅
    if(angle > 180) angle = 180;

    // 角度转脉宽
    pulse = PWM_MIN + (angle * 2000 / 180);

    // 设置比较值（修改占空比）
    TIM_SetCompare1(TIM2, pulse); //20ms
}
/*
 * servo.c
 *
 *  Created on: 2026年2月8日
 *      Author: Elaina
 */


