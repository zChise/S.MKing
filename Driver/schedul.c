/*
 * schedul.c
 *
 *  Created on: 2026Äê2ÔÂ6ÈÕ
 *      Author: Elaina
 */

#include "base.h"

unsigned char task_num;
typedef struct
{
    void (*task_func)(void);
    unsigned long int rate_ms;
    unsigned long int last_run;
}task_t;


task_t scheduler_task[]={
        {lcd_proc,100,0},
        {key_proc,10,0},
        {as608_proc,20,0},
        {AT24_proc, 1000, 0},
        {esp8266_proc, 2, 0},
};



void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;
            scheduler_task[i].task_func();
        }
    }
}
