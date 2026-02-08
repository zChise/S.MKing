/*
 * shcdeul.c
 *
 *  Created on: 2026年2月6日
 *      Author: Elaina
 */

#include "base.h"

unsigned char task_num;
typedef struct
{
    void (*task_func)(void);//任务函数
    unsigned long int rate_ms;  //任务执行周期
    unsigned long int last_run; //任务上次运行的时间
}task_t;


task_t scheduler_task[]={
        {lcd_proc,100,0},//屏幕处理函数，100毫秒执行一次，0秒开始执行
        {key_proc,10,0},//按键处理函数，10毫秒执行一次，0秒开始执行
//        {lock_proc,30,0},//门锁处理函数（舵机开关）
        {as608_proc,20,0},
//        {esp8266_proc,2,0},
        {AT24_proc, 1000, 0},
};



void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);  //计算任务数量
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;  //更新当前系统时间
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;//记录本次运行时间
            scheduler_task[i].task_func();//运行任务
        }
    }
}

