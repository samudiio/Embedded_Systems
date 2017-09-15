/*
 * Tasks.c
 *
 * Created: 6/21/2016 7:25:43 PM
 *  Author: Francisco Martinez
 */ 

#include "app_scheduler.h"
#include "led.h"
#include "Tasks.h"
#include <stdio.h>

uint8_t u8100ms_Ctr=0;
uint8_t u8100ms_Ctr2=0;

/*------------------------------------------------------------------------------
 *         Exported Variables
 *----------------------------------------------------------------------------*/
uint8_t ExtTsk_Activated = 0;


void vfnTsk_1ms(void)
{
    printf("**********************\n\r");
    printf("* 1ms Task Execution *\n\r");
    printf("**********************\n\r\n");
    vfnSchedulePoint();
}

void vfnTsk_2msA(void)
{
    printf("**********************\n\r");
    printf("* 2ms A Task Execution *\n\r");
    printf("**********************\n\r\n");
    vfnSchedulePoint();
}

void vfnTsk_2msB(void)
{
    printf("**********************\n\r");
    printf("* 2ms B Task Execution *\n\r");
    printf("**********************\n\r\n");
    vfnSchedulePoint();
}

void vfnTsk_10ms(void)
{
    printf("**********************\n\r");
    printf("* 10ms Task Execution *\n\r");
    printf("**********************\n\r\n");
    vfnSchedulePoint();
}

void vfnTsk_50ms(void)
{
    static uint8_t u8500ms_Ctr=0;

    u8500ms_Ctr++;

    if (25 <= u8500ms_Ctr)
    {
        u8500ms_Ctr = 0;
        LED_Toggle( 0 );
    }

    printf("**********************\n\r");
    printf("* 50ms Task Execution *\n\r");
    printf("**********************\n\r\n");

    vfnSchedulePoint();
}

void vfnTsk_100ms(void)
{
    u8100ms_Ctr++;
    u8100ms_Ctr2++;

    if (5 <= u8100ms_Ctr)
    {
        u8100ms_Ctr = 0;
        LED_Toggle( 1 );
    }
    if (10 <= u8100ms_Ctr2)
    {
        u8100ms_Ctr2 = 0;
    }

    printf("**********************\n\r");
    printf("* 100ms Task Execution *\n\r");
    printf("**********************\n\r\n");
    vfnSchedulePoint();

}


/**
 *  \brief External Triggered Task.
 *
 *  Execution of this task will be deppendent on the previouly activation of it-
 *  in Timer0 ISR handler, vfnSchedulePoint() will check priority of current active-
 *  task and decide if run this task or not
 */
void vfnTsk_ExtTriggered(void)
{
    //ExtTsk_Activated = 1;
    //LED_Toggle( 1 );
    printf("**********************\n\r");
    printf("* EXTERNAL Task Execution *\n\r");
    printf("**********************\n\r\n");
}

